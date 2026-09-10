# Copyright (c) 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: BSD-3-Clause
#
# ti-zephyr documentation build configuration file.
# Structure mirrors zephyrproject-rtos/zephyr's doc/conf.py (see
# ../zephyr/doc/conf.py in a west workspace) with the kernel-only
# sections (multi-version dropdown, LaTeX/PDF, manifest table, sitemap)
# left out for this minimal framework pass. Content is added later;
# this file only has to make an empty doc/ tree build cleanly.
#
# Reference: https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import re
import sys
from pathlib import Path

TI_ZEPHYR_BASE = Path(__file__).resolve().parents[1]
TI_ZEPHYR_BUILD = Path(os.environ.get("OUTPUT_DIR", str(TI_ZEPHYR_BASE / "doc" / "_build")))

# Add the '_extensions' directory to sys.path, to enable finding Sphinx
# extensions within.
sys.path.insert(0, str(TI_ZEPHYR_BASE / "doc" / "_extensions"))

# Add the '_scripts' directory to sys.path, to enable finding utility
# modules.
sys.path.insert(0, str(TI_ZEPHYR_BASE / "doc" / "_scripts"))

import redirects  # noqa: E402

# -- Project --------------------------------------------------------------

project = "TI Zephyr SDK"
copyright = "2026 Texas Instruments Incorporated"
author = "Texas Instruments Incorporated"

# parse version from 'VERSION' file (same format as upstream Zephyr)
with open(TI_ZEPHYR_BASE / "VERSION") as f:
    m = re.match(
        (
            r"^VERSION_MAJOR\s*=\s*(\d+)$\n"
            + r"^VERSION_MINOR\s*=\s*(\d+)$\n"
            + r"^PATCHLEVEL\s*=\s*(\d+)$\n"
            + r"^VERSION_TWEAK\s*=\s*\d+$\n"
            + r"^EXTRAVERSION\s*=\s*(.*)$"
        ),
        f.read(),
        re.MULTILINE,
    )

    if not m:
        sys.stderr.write("Warning: Could not extract ti-zephyr version\n")
        version = "Unknown"
    else:
        major, minor, patch, extra = m.groups(1)
        version = ".".join((major, minor, patch))
        if extra:
            version += "-" + extra

release = version

# -- General configuration ------------------------------------------------

extensions = [
    "sphinx_rtd_theme",
    "sphinx.ext.todo",
    "sphinx.ext.extlinks",
    "sphinx.ext.autodoc",
    "sphinx.ext.graphviz",
    "sphinxcontrib.jquery",
    "sphinxcontrib.programoutput",
    "zephyr.application",
    "zephyr.html_redirects",
    # "zephyr.kconfig" is re-enabled once ti-zephyr has its own root
    # Kconfig tree to point kconfig_ext_paths at — needs real Kconfig
    # content, not just framework.
    "zephyr.dtcompatible-role",
    "zephyr.link-roles",
    "sphinx_tabs.tabs",
    "zephyr.doxyrunner",
    "zephyr.doxybridge",
    "zephyr.doxytooltip",
    # "zephyr.gh_utils" needs a MAINTAINERS.yml + get_maintainer.py —
    # re-enable once ti-zephyr has repo-maintainer tracking set up.
    "notfound.extension",
    "sphinx_copybutton",
    "sphinx_togglebutton",
    "zephyr.external_content",
    # "zephyr.domain" (the code-sample / hardware-features domain) hard-
    # imports zephyr.gh_utils, which needs a MAINTAINERS.yml + get_maintainer.py
    # at import time — real repo-scale infra ti-zephyr doesn't have yet.
    # Re-enable once that lands.
    "zephyr.api_overview",
]

templates_path = ["_templates"]

exclude_patterns = ["_build"]

pygments_style = "sphinx"
highlight_language = "none"

todo_include_todos = False

nitpick_ignore = [
    # ignore C standard identifiers (not defined in ti-zephyr docs)
    ("c:identifier", "FILE"),
    ("c:identifier", "int8_t"),
    ("c:identifier", "int16_t"),
    ("c:identifier", "int32_t"),
    ("c:identifier", "int64_t"),
    ("c:identifier", "intptr_t"),
    ("c:identifier", "off_t"),
    ("c:identifier", "size_t"),
    ("c:identifier", "ssize_t"),
    ("c:identifier", "time_t"),
    ("c:identifier", "uint8_t"),
    ("c:identifier", "uint16_t"),
    ("c:identifier", "uint32_t"),
    ("c:identifier", "uint64_t"),
    ("c:identifier", "uintptr_t"),
    ("c:identifier", "va_list"),
]

rst_epilog = """
.. include:: /substitutions.txt
"""

# -- Options for HTML output ----------------------------------------------

html_theme = "sphinx_rtd_theme"
html_theme_options = {
    "logo_only": True,
    "prev_next_buttons_location": None,
    "navigation_depth": 5,
}
html_title = "TI Zephyr SDK Documentation"
html_logo = str(TI_ZEPHYR_BASE / "doc" / "_static" / "images" / "logo.svg")
html_favicon = str(TI_ZEPHYR_BASE / "doc" / "_static" / "images" / "favicon.png")
html_static_path = [str(TI_ZEPHYR_BASE / "doc" / "_static")]
html_last_updated_fmt = "%b %d, %Y"
html_domain_indices = False
html_split_index = True
html_show_sourcelink = False
html_show_sphinx = False
html_search_scorer = str(TI_ZEPHYR_BASE / "doc" / "_static" / "js" / "scorer.js")
html_additional_pages = {
    "gsearch": "gsearch.html"
}

is_release = tags.has("release")  # pylint: disable=undefined-variable  # noqa: F821
docs_title = "Docs / {}".format(version if is_release else "Latest")
html_context = {
    "show_license": True,
    "docs_title": docs_title,
    "is_release": is_release,
    "current_version": version,
    "versions": (
        ("latest", "/"),
    ),
    "display_gh_links": False,
    "reference_links": {
        "API": "doxygen/html/index.html",
    },
}

# -- Options for zephyr.doxyrunner plugin ---------------------------------

doxyrunner_doxygen = os.environ.get("DOXYGEN_EXECUTABLE", "doxygen")
doxyrunner_projects = {
    "ti-zephyr": {
        "doxyfile": TI_ZEPHYR_BASE / "doc" / "ti-zephyr.doxyfile.in",
        "outdir": TI_ZEPHYR_BUILD / "doxygen",
        "fmt": True,
        "fmt_vars": {
            "TI_ZEPHYR_BASE": str(TI_ZEPHYR_BASE),
            "TI_ZEPHYR_VERSION": version,
        },
        "outdir_var": "DOXY_OUT",
    },
}

# -- Options for zephyr.doxybridge plugin ---------------------------------

doxybridge_projects = {"ti-zephyr": doxyrunner_projects["ti-zephyr"]["outdir"]}

# -- Options for html_redirect plugin -------------------------------------

html_redirect_pages = redirects.REDIRECTS

# -- Options for zephyr.link-roles ----------------------------------------

link_roles_manifest_project = "ti-zephyr"
link_roles_manifest_baseurl = "https://github.com/TexasInstruments/ti-zephyr"

# -- Options for notfound.extension ---------------------------------------

notfound_urls_prefix = "/latest/"

# -- Options for zephyr.gh_utils ------------------------------------------
# (extension disabled above until ti-zephyr has MAINTAINERS.yml)
#
# gh_link_version = "main"
# gh_link_base_url = "https://github.com/TexasInstruments/ti-zephyr"
# gh_link_prefixes = {
#     "samples/.*": "",
#     "boards/.*": "",
#     ".*": "doc",
# }

# -- Options for zephyr.kconfig -------------------------------------------
# (extension disabled above until ti-zephyr has its own root Kconfig tree)
#
# kconfig_generate_db = True
# kconfig_ext_paths = [TI_ZEPHYR_BASE]
# kconfig_gh_link_base_url = "https://github.com/TexasInstruments/ti-zephyr"
# kconfig_zephyr_version = "main"

# -- Options for zephyr.external_content ----------------------------------

external_content_contents = [
    (TI_ZEPHYR_BASE / "doc", "[!_]*"),
    (TI_ZEPHYR_BASE, "boards/*/*/doc/*.rst"),
    (TI_ZEPHYR_BASE, "samples/*/*.rst"),
    (TI_ZEPHYR_BASE, "samples/*/doc"),
]

# -- Options for sphinx.ext.graphviz --------------------------------------

graphviz_dot = os.environ.get("DOT_EXECUTABLE", "dot")
graphviz_output_format = "svg"

# -- Options for sphinx_copybutton ----------------------------------------

copybutton_prompt_text = r"\$ |uart:~\$ "
copybutton_prompt_is_regexp = True

# -- Linkcheck options ----------------------------------------------------

linkcheck_timeout = 30
linkcheck_workers = 10
linkcheck_anchors = False

# -- Options for zephyr.api_overview --------------------------------------

api_overview_doxygen_out_dir = str(doxyrunner_projects["ti-zephyr"]["outdir"])
api_overview_base_url = "https://github.com/TexasInstruments/ti-zephyr"


def setup(app):
    # theme customizations
    app.add_css_file("css/custom.css")
    app.add_js_file("js/custom.js")
