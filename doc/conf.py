"""Sphinx configuration for TI Zephyr documentation."""

import json
import os
import re
import sys
from pathlib import Path

import yaml
from sphinx.errors import ConfigError

REPO_BASE = Path(__file__).resolve().parent.parent
DOC_BASE  = Path(__file__).resolve().parent

sys.path.insert(0, str(DOC_BASE / "_extensions"))

# -- Project information -------------------------------------------------------

project   = "TI Zephyr"
author    = "Texas Instruments"
copyright = "Texas Instruments Incorporated"

_version_file = REPO_BASE / "VERSION"
if _version_file.exists():
    _v = {}
    for line in _version_file.read_text().splitlines():
        if "=" in line:
            k, _, v = line.partition("=")
            _v[k.strip()] = v.strip()
    version = ".".join(
        filter(None, [_v.get("VERSION_MAJOR"), _v.get("VERSION_MINOR"), _v.get("PATCHLEVEL")])
    )
    release = version
else:
    version = "1.00.00"
    release = "1.00.00"

# -- SoC selection -------------------------------------------------------------
# The docs are built for exactly one SoC family:
#   make html SOC=<soc>        (or SOC=<soc> sphinx-build ...)
# doc/socs.json is the single list of families; the CI matrix and the landing
# page generator read it too. The active family is also a Sphinx tag, so
# family-specific text can go in ``.. only:: <soc>`` blocks (one per family).

SOCS = json.loads((DOC_BASE / "socs.json").read_text(encoding="utf-8"))

soc = os.environ.get("SOC", "").strip().lower()
if soc not in SOCS:
    raise ConfigError(
        f"SOC must be set to one of: {', '.join(SOCS)} (got '{soc}'). "
        "Example: make html SOC=mspm0"
    )
tags.add(soc)  # noqa: F821 - injected by Sphinx

_soc_name = SOCS[soc]["name"]
project   = f"Zephyr SDK for {_soc_name}"
html_title = f"{project} {release} documentation"

# -- General configuration -----------------------------------------------------

extensions = [
    "external_content",
    "sphinx.ext.autosectionlabel",
    "ti_ref",
    "ti_hw_features",
]

# Zephyr tree the board hardware feature tables are read from. Empty means
# $ZEPHYR_BASE, else the zephyr/ folder next to ti-zephyr in the west workspace.
ti_zephyr_base = None

source_suffix   = {".rst": "restructuredtext"}
master_doc      = "index"
templates_path  = ["_templates"]
exclude_patterns = ["_build", "_extensions", "Thumbs.db", ".DS_Store"]

_subs_file = DOC_BASE / "substitutions.txt"
rst_epilog = _subs_file.read_text() if _subs_file.exists() else ""
rst_epilog += f"""
.. |soc_name| replace:: {_soc_name}
.. |soc_devices| replace:: {SOCS[soc]["devices"]}
.. |soc_board_support| replace:: :doc:`{_soc_name} board support page </board_support/{soc}/index>`
"""

autosectionlabel_prefix_document = True

# toc.excluded: the home toctree names components/index, which is excluded for
# SoCs without any component page (see SoC filtering below).
suppress_warnings = ["toc.not_included", "toc.excluded"]

# -- External content ----------------------------------------------------------
# (DOC_BASE, "[!_]*") protects all non-underscore-prefixed doc files from the
# stale-file deletion pass. external_content_keep protects _static/, etc.

external_content_contents = [
    (DOC_BASE, "[!_]*"),
    (REPO_BASE, "samples/**/*.rst"),
    (REPO_BASE, "tests/**/*.rst"),
]
exclude_patterns += ["samples/README.rst", "tests/README.rst"]

external_content_keep = [
    "_static/**/*",
    "_templates/**/*",
    "_extensions/**/*",
]

# -- SoC filtering -------------------------------------------------------------
# A page that is not built cannot leak into the sidebar, prev/next links or the
# search index, so everything that does not apply to the active SoC is added to
# exclude_patterns. Listings use ``:glob:`` toctrees, so excluded pages simply
# drop out of them.
#
# 1. Per-SoC pages: board_support/<soc>/ and getting_started/<soc>/.
# 2. Samples, tests, components and tools: a page (README.rst for samples and
#    tests, index.rst for components and tools) may declare the SoCs it applies
#    to with an (invisible) comment line, e.g.
#        .. ti-socs: mspm0 am13
#    For components and tools, no line means all SoCs. For a sample or test
#    page, no line means the SoCs owning a board in the platform_allow of its
#    sample.yaml / testcase.yaml / tests.yaml, and a page with neither is an
#    error. A directory with pages below it is a category page: it needs
#    neither, and is kept only while at least one child is kept. In samples/
#    and tests/ the category README.rst is optional; when missing it is
#    generated into doc/ with the title taken from the directory name.
#
# The explicit labels (.. _label:) of every page excluded this way are collected
# in ti_unavailable_labels, so the :ti-ref: role (see _extensions/ti_ref.py) can
# link to such a page when it is built and fall back to plain text when not.

ti_unavailable_labels = []

for _other in SOCS:
    if _other != soc:
        exclude_patterns += [f"board_support/{_other}/*", f"getting_started/{_other}/*"]

for _page in (f"board_support/{soc}/index.rst", f"getting_started/{soc}"):
    if not (DOC_BASE / _page).exists():
        raise ConfigError(f"Missing page doc/{_page} for SoC '{soc}'")

_TI_SOCS_RE = re.compile(r"^\.\. ti-socs:[ \t]*(.+?)[ \t]*$", re.MULTILINE)
_LABEL_RE = re.compile(r"^\.\. _([^:\n]+):[ \t]*$", re.MULTILINE)


_TWISTER_YAMLS = ("sample.yaml", "testcase.yaml", "tests.yaml")


def _yaml_boards(directory):
    """Boards in the platform_allow of the twister yaml files, or None if none."""
    boards = None
    for name in _TWISTER_YAMLS:
        path = directory / name
        if not path.is_file():
            continue
        data = yaml.safe_load(path.read_text(encoding="utf-8")) or {}
        for scope in [data.get("common"), *(data.get("tests") or {}).values()]:
            allow = (scope or {}).get("platform_allow")
            if allow:
                boards = (boards or set()) | set(
                    allow.split() if isinstance(allow, str) else allow
                )
    return boards


def _supports_soc(page, derive_from_yaml):
    match = _TI_SOCS_RE.search(page.read_text(encoding="utf-8"))
    if match:
        declared = match.group(1).split()
        unknown = [s for s in declared if s not in SOCS]
        if unknown:
            raise ConfigError(
                f"{page.relative_to(REPO_BASE)}: unknown SoC(s) in '.. ti-socs:': "
                f"{', '.join(unknown)} (known: {', '.join(SOCS)})"
            )
        return soc in declared
    if not derive_from_yaml:
        return True
    boards = _yaml_boards(page.parent)
    if boards is None:
        raise ConfigError(
            f"{page.relative_to(REPO_BASE)}: cannot tell which SoCs this page "
            f"applies to; add a '.. ti-socs:' line or a platform_allow list to "
            f"{' / '.join(_TWISTER_YAMLS)} next to it"
        )
    return bool(boards & set(SOCS[soc]["boards"]))


def _exclude(page, base):
    exclude_patterns.append(page.relative_to(base).as_posix())
    ti_unavailable_labels.extend(
        " ".join(label.lower().split())
        for label in _LABEL_RE.findall(page.read_text(encoding="utf-8"))
    )


_ACRONYMS = frozenset(
    "adc dac pwm gpio i2c spi uart can usb rtc dma led lcd".split()
)

# Directories of samples/ and tests/ that hold pages but no README.rst of
# their own; their category page is generated into doc/ (see setup() below).
_generated_categories = []
_GENERATED_MARKER = ".. ti-generated-category"

# external_content treats whatever is in doc/ as source and never deletes it, so
# generated pages left over from a build for another SoC are removed up front.
for _top in ("samples", "tests"):
    for _old in (DOC_BASE / _top).rglob("README.rst"):
        if _old.read_text(encoding="utf-8").startswith(_GENERATED_MARKER):
            _old.unlink()


def _prune(directory, page_name, base, twister_tree=False):
    """Exclude pages not applying to ``soc``; return True if the page is kept.

    ``page_name`` is the page file in each directory; excluded paths are made
    relative to ``base``, the directory that maps onto doc/ in the build. For
    the samples/tests tree (``twister_tree``), a leaf page without a
    '.. ti-socs:' line takes its SoCs from the platform_allow of its twister
    yaml files, and a directory with pages below it but no page of its own gets
    a generated category page instead of an error.
    """
    page = directory / page_name
    if not any(directory.rglob(page_name)):
        return False

    children = [d for d in sorted(directory.iterdir()) if d.is_dir()]
    child_kept = [_prune(d, page_name, base, twister_tree) for d in children]

    if not page.is_file():
        if not twister_tree:
            raise ConfigError(
                f"{directory.relative_to(REPO_BASE)} contains {page_name} pages "
                f"but has no {page_name} category page of its own"
            )
        if any(child_kept):
            _generated_categories.append(directory)
        return any(child_kept)

    is_category = any(any(d.rglob(page_name)) for d in children)
    keep = _supports_soc(page, twister_tree and not is_category) and (
        any(child_kept) or not is_category
    )
    if not keep:
        _exclude(page, base)
    return keep


# The samples/ and tests/ landing pages list their children with a glob
# toctree; a glob that matches nothing is an unsuppressible warning, so a
# landing page with no page left for this SoC is excluded as well.
for _top in ("samples", "tests"):
    _kept = [
        _prune(_d, "README.rst", REPO_BASE, twister_tree=True)
        for _d in sorted((REPO_BASE / _top).iterdir())
        if _d.is_dir()
    ]
    if not any(_kept):
        _exclude(REPO_BASE / _top / "index.rst", REPO_BASE)

# The top-level components/ and tools/ hubs are pruned too, so a section with
# nothing for this SoC disappears from the site instead of being left empty.
for _top in ("components", "tools"):
    _prune(DOC_BASE / _top, "index.rst", DOC_BASE)

def _write_category_pages(app):
    for directory in _generated_categories:
        title = " ".join(
            w.upper() if w in _ACRONYMS else w.capitalize()
            for w in directory.name.split("_")
        )
        page = DOC_BASE / directory.relative_to(REPO_BASE) / "README.rst"
        page.parent.mkdir(parents=True, exist_ok=True)
        page.write_text(
            f"{_GENERATED_MARKER}\n\n{title}\n{'#' * len(title)}\n\n"
            ".. toctree::\n   :titlesonly:\n   :maxdepth: 1\n   :glob:\n\n"
            "   */README\n",
            encoding="utf-8",
        )


def setup(app):
    # Priority after external_content, which deletes every file in doc/ that is
    # not copied from the repository.
    app.connect("builder-inited", _write_category_pages, priority=900)


# -- HTML output ---------------------------------------------------------------

html_theme = "sphinx_rtd_theme"
html_logo  = "_static/ti_logo.png"

html_static_path = ["_static"]
html_css_files   = ["custom.css"]

html_theme_options = {
    "collapse_navigation": False,
    "sticky_navigation":   True,
    "navigation_depth":    5,
    "logo_only":           False,
    "titles_only":         False,
}

html_show_sphinx     = False
html_show_sourcelink = False

html_context = {
    "display_github": False,
}
