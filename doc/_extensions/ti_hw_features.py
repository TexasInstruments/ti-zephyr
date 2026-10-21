"""Hardware feature tables for the Board Support pages.

``.. ti-hw-features:: <board>`` renders the devicetree nodes of every board
target of ``<board>`` as a Type / Location / Description / Compatible table,
the same table the upstream Zephyr board pages show. The devicetree is not
kept in ti-zephyr: it is read from the Zephyr tree that sits next to it in the
west workspace (or from ``$ZEPHYR_BASE``) by running the C preprocessor and
Zephyr's own ``devicetree.edtlib`` on the board's ``.dts``.

``.. ti-hw-features-legend::`` renders the key to the chips and counters.

When no Zephyr tree is found, the directive leaves a note in the page instead
of failing, so the docs can still be built without a west workspace.
"""

import logging
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import yaml
from docutils import nodes
from sphinx.util.docutils import SphinxDirective

logger = logging.getLogger(__name__)

_ACRONYM_RE = re.compile(r"([a-zA-Z0-9-]+)\s*\((.*?)\)")
_ACRONYM_UPPER_RE = re.compile(r"(\b[A-Z0-9-]+)\s*\((.*?)\)")

_COLUMN_WIDTHS = (15, 12, 53, 20)
_HEADERS = ("Type", "Location", "Description", "Compatible")

_cache = {}


def _zephyr_base(app):
    configured = app.config.ti_zephyr_base or os.environ.get("ZEPHYR_BASE")
    base = Path(configured) if configured else Path(app.confdir).parent.parent / "zephyr"
    base = base.resolve()
    return base if (base / "boards").is_dir() and (base / "dts" / "bindings").is_dir() else None


def _dts_roots(zephyr_base, ti_zephyr):
    """DTS roots in the order Zephyr uses: ti-zephyr, modules, then Zephyr itself."""
    workspace = zephyr_base.parent
    roots = [ti_zephyr]
    for pattern in (
        "modules/*/zephyr/module.yml",
        "modules/*/*/zephyr/module.yml",
        "modules/*/*/*/zephyr/module.yml",
        "bootloader/*/zephyr/module.yml",
        "tools/*/zephyr/module.yml",
    ):
        for module_yml in sorted(workspace.glob(pattern)):
            data = yaml.safe_load(module_yml.read_text(encoding="utf-8")) or {}
            dts_root = ((data.get("build") or {}).get("settings") or {}).get("dts_root")
            if dts_root:
                roots.append((module_yml.parent.parent / dts_root).resolve())
    roots.append(zephyr_base)
    return list(dict.fromkeys(roots))


def _board_targets(roots, board):
    """Return (board_dir, [(identifier, arch, ram_kib, flash_kib, dts_path), ...])."""
    for root in roots:
        for board_dir in sorted((root / "boards").glob(f"*/{board}")):
            if not (board_dir / "board.yml").is_file():
                continue
            targets = []
            for target_yaml in sorted(board_dir.glob("*.yaml")):
                data = yaml.safe_load(target_yaml.read_text(encoding="utf-8")) or {}
                ident = data["identifier"]
                dts = board_dir / f"{ident.replace('/', '_')}.dts"
                targets.append((ident, data.get("arch", ""), data.get("ram"), data.get("flash"), dts))
            return board_dir, targets
    return None, []


def _preprocess(dts, arch, roots, zephyr_base, out):
    cpp = shutil.which("cpp")
    if cpp is None:
        raise RuntimeError("the C preprocessor 'cpp' is needed to read the board devicetree")

    arch_dirs = sorted(
        p.name
        for p in (zephyr_base / "dts").iterdir()
        if p.is_dir() and p.name not in ("common", "vendor", "bindings")
    )
    if arch in arch_dirs:
        arch_dirs.remove(arch)
        arch_dirs.insert(0, arch)

    includes = []
    for root in roots:
        for sub in ("include", "include/zephyr", "dts/common", "dts/vendor",
                    *(f"dts/{a}" for a in arch_dirs), "dts"):
            if (root / sub).is_dir():
                includes += ["-isystem", str(root / sub)]

    result = subprocess.run(
        [cpp, "-x", "assembler-with-cpp", "-nostdinc", "-undef", "-D__DTS__", "-E",
         *includes, "-o", str(out), str(dts)],
        capture_output=True, text=True,
    )
    if result.returncode:
        raise RuntimeError(f"preprocessing {dts} failed:\n{result.stderr.strip()}")


def _first_sentence(text):
    if not text:
        return ""
    first = text.replace("\n", " ").split("  ")[0].strip()
    match = re.search(r"(.*?)\.(?:\s|$)", first)
    return match.group(1).strip() if match else first


def _load_target(zephyr_base, roots, arch, dts):
    key = str(dts)
    if key not in _cache:
        sys.path.insert(0, str(zephyr_base / "scripts" / "dts" / "python-devicetree" / "src"))
        from devicetree import edtlib

        logging.getLogger("devicetree").setLevel(logging.ERROR)
        bindings_dirs = [str(r / "dts" / "bindings") for r in roots if (r / "dts" / "bindings").is_dir()]
        with tempfile.TemporaryDirectory() as tmp:
            pre = Path(tmp) / "board.dts.pre"
            _preprocess(dts, arch, roots, zephyr_base, pre)
            edt = edtlib.EDT(str(pre), bindings_dirs, warn_reg_unit_address_mismatch=False)
        _cache[key] = _features(edt, zephyr_base, roots), {n.filename for n in edt.nodes}
    return _cache[key]


def _features(edt, zephyr_base, roots):
    """Group the nodes of ``edt`` as {binding type: {compatible: info}}."""
    bindings = zephyr_base / "dts" / "bindings"
    features = {}
    for node in edt.nodes:
        compat = node.matching_compat
        if node.binding_path is None or compat is None or compat.startswith("zephyr,"):
            continue

        binding_path = Path(node.binding_path)
        binding_type = (
            binding_path.relative_to(bindings).parts[0]
            if binding_path.is_relative_to(bindings)
            else "misc"
        )

        location = None
        for root in roots:
            if Path(node.filename).is_relative_to(root):
                is_board = Path(node.filename).relative_to(root).parts[0] == "boards"
                location = "board" if is_board else "soc"
                break

        info = features.setdefault(binding_type, {}).setdefault(
            compat,
            {
                "title": node.title,
                "description": _first_sentence(node.description),
                "locations": set(),
                "okay": 0,
                "disabled": 0,
            },
        )
        if location:
            info["locations"].add(location)
        info["okay" if node.status == "okay" else "disabled"] += 1
    return features


def _binding_types(zephyr_base):
    types = {}
    for line in (zephyr_base / "dts" / "bindings" / "binding-types.txt").read_text(
        encoding="utf-8"
    ).splitlines():
        line = line.strip()
        if line and not line.startswith("#") and "\t" in line:
            key, value = line.split("\t", 1)
            types[key] = value
    return types


def _with_acronyms(text, uppercase_only=False):
    pattern = _ACRONYM_UPPER_RE if uppercase_only else _ACRONYM_RE
    result = nodes.inline()
    last = 0
    for match in pattern.finditer(text):
        result += nodes.Text(text[last:match.start()])
        result += nodes.abbreviation(match.group(1), match.group(1), explanation=match.group(2))
        last = match.end()
    result += nodes.Text(text[last:])
    return result


def _memory(kib):
    return f"{kib // 1024} MiB" if kib >= 1024 and kib % 1024 == 0 else f"{kib} KiB"


def _memory_paragraph(ram, flash):
    parts = []
    if ram:
        parts.append(f"{_memory(ram)} of RAM")
    if flash:
        parts.append(f"{_memory(flash)} of Flash")
    if not parts:
        return None
    return nodes.paragraph(
        text=f"On-target memory for this board target: {', '.join(parts)}."
    )


def _entry(*children, classes=None, morerows=None):
    entry = nodes.entry(classes=classes or [])
    if morerows:
        entry["morerows"] = morerows
    for child in children:
        entry += child
    return entry


def _table(features, binding_types):
    table = nodes.table(classes=["colwidths-given", "hardware-features", "force-wrap"])
    tgroup = nodes.tgroup(cols=len(_COLUMN_WIDTHS))
    for width in _COLUMN_WIDTHS:
        tgroup += nodes.colspec(colwidth=width)

    head = nodes.row()
    for header in _HEADERS:
        head += _entry(nodes.paragraph(text=header))
    thead = nodes.thead()
    thead += head
    tgroup += thead

    tbody = nodes.tbody()
    for binding_type in sorted(features, key=lambda t: (t != "cpu", t)):
        compats = sorted(features[binding_type])
        for i, compat in enumerate(compats):
            info = features[binding_type][compat]
            row = nodes.row()

            if i == 0:
                label = _with_acronyms(binding_types.get(binding_type, binding_type))
                row += _entry(nodes.paragraph("", "", label), morerows=len(compats) - 1)

            location = nodes.paragraph()
            if "board" in info["locations"]:
                location += nodes.inline(text="on-board", classes=["location-chip", "onboard"])
            elif "soc" in info["locations"]:
                location += nodes.inline(text="on-chip", classes=["location-chip", "onchip"])
            row += _entry(location)

            description = nodes.paragraph(classes=["status"])
            if info["title"]:
                description += _with_acronyms(info["title"], uppercase_only=True)
            else:
                description += nodes.Text(info["description"])
            for kind in ("okay", "disabled"):
                if info[kind]:
                    description += nodes.inline(
                        text=str(info[kind]), classes=["count", f"{kind}-count"]
                    )
            row += _entry(description)

            row += _entry(nodes.paragraph("", "", nodes.literal(text=compat)))
            tbody += row
    tgroup += tbody
    table += tgroup
    return table


def _note(text):
    note = nodes.admonition(classes=["warning"])
    note += nodes.title(text="Note")
    note += nodes.paragraph(text=text)
    return note


class TiHwFeatures(SphinxDirective):
    """Hardware feature table of one board, read from its devicetree."""

    required_arguments = 1
    has_content = False

    def run(self):
        board = self.arguments[0]
        zephyr_base = _zephyr_base(self.env.app)
        if zephyr_base is None:
            logger.info(
                "%s: no Zephyr tree found next to ti-zephyr (set ZEPHYR_BASE); "
                "hardware features of %s not generated", self.env.docname, board
            )
            return [_note(
                "The list of hardware features was not generated because the Zephyr "
                "tree was not available when this documentation was built."
            )]

        ti_zephyr = Path(self.env.app.confdir).parent.resolve()
        roots = _dts_roots(zephyr_base, ti_zephyr)
        board_dir, targets = _board_targets(roots, board)
        if board_dir is None or not targets:
            raise self.error(f"board '{board}' not found in the boards/ directories of {roots}")

        binding_types = _binding_types(zephyr_base)
        result = []
        for ident, arch, ram, flash, dts in targets:
            if not dts.is_file():
                raise self.error(f"devicetree {dts} of board target '{ident}' not found")
            try:
                features, dts_files = _load_target(zephyr_base, roots, arch, dts)
            except Exception as err:
                raise self.error(f"cannot read the devicetree of '{ident}': {err}")
            for dts_file in dts_files:
                self.env.note_dependency(dts_file)

            if len(targets) > 1:
                result.append(nodes.rubric(ident, "", nodes.literal(text=ident), nodes.Text(" target")))
            memory = _memory_paragraph(ram, flash)
            if memory:
                result.append(memory)
            result.append(_table(features, binding_types))
        return result


class TiHwFeaturesLegend(SphinxDirective):
    """Key to the chips and counters used in the hardware feature tables."""

    has_content = False

    def run(self):
        def item(label, text):
            entry = nodes.list_item()
            paragraph = nodes.paragraph()
            paragraph += label
            paragraph += nodes.Text(f" {text}")
            entry += paragraph
            return entry

        def chip(text, kind):
            return nodes.inline(text=text, classes=["location-chip", kind])

        def count(text, kind):
            return nodes.inline(text=text, classes=["count", kind])

        def join(first, second):
            both = nodes.inline()
            both += first
            both += nodes.Text(" / ")
            both += second
            return both

        items = nodes.bullet_list()
        items += item(
            join(chip("on-chip", "onchip"), chip("on-board", "onboard")),
            "Feature integrated in the SoC / present on the board.",
        )
        items += item(
            join(count("2", "okay-count"), count("2", "disabled-count")),
            "Number of instances that are enabled / disabled.",
        )
        items += item(
            nodes.literal(text="vnd,foo"),
            "Compatible string of the devicetree binding matching the feature.",
        )
        legend = nodes.admonition(classes=["legend"])
        legend += nodes.title(text="Legend")
        legend += items
        return [legend]


def setup(app):
    app.add_config_value("ti_zephyr_base", None, "env")
    app.add_directive("ti-hw-features", TiHwFeatures)
    app.add_directive("ti-hw-features-legend", TiHwFeaturesLegend)
    return {"parallel_read_safe": True, "parallel_write_safe": True}
