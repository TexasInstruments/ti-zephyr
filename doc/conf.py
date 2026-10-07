"""Sphinx configuration for TI Zephyr documentation."""

import os
import sys
from pathlib import Path

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
    version = "main"
    release = "main"

# -- SoC selection -------------------------------------------------------------
# ``make html SOC=<soc>`` (or ``SOC=<soc> sphinx-build ...``) builds the docs
# for one SoC family. Family-specific content goes in ``.. only:: <soc>``
# blocks. Without SOC, every family tag is set so all content is included.
#
# Keep in sync with the matrix in .github/workflows/docs.yml.

SOCS = {
    "mspm0":  "MSPM0",
    "mspm33": "MSPM33",
    "am13":   "AM13",
    "am62l":  "AM62L",
}

soc = os.environ.get("SOC", "").strip().lower()
if soc and soc not in SOCS:
    raise ConfigError(f"Unknown SOC '{soc}', expected one of: {', '.join(SOCS)}")

for _s in [soc] if soc else SOCS:
    tags.add(_s)  # noqa: F821 - injected by Sphinx

if soc:
    html_title = f"{project} for {SOCS[soc]} {release} documentation"

# -- General configuration -----------------------------------------------------

extensions = [
    "external_content",
    "sphinx.ext.autosectionlabel",
]

source_suffix   = {".rst": "restructuredtext"}
master_doc      = "index"
templates_path  = ["_templates"]
exclude_patterns = ["_build", "_extensions", "Thumbs.db", ".DS_Store"]

_subs_file = DOC_BASE / "substitutions.txt"
if _subs_file.exists():
    rst_epilog = _subs_file.read_text()

autosectionlabel_prefix_document = True

# -- External content ----------------------------------------------------------
# The extension copies external RST files into the Sphinx source dir before
# the build starts, then removes any files it didn't copy (stale cleanup).
#
# (DOC_BASE, "[!_]*") protects all non-underscore-prefixed doc files (index.rst,
# conf.py, Makefile, etc.) from the stale-file deletion pass — same technique
# as Zephyr's (ZEPHYR_BASE / "doc", "[!_]*").
#
# external_content_keep protects _static/, _templates/, _extensions/ which
# are prefixed with _ and therefore skipped by the [!_]* glob.

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
