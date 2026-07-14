# Copyright (c) 2026 Texas Instruments Incorporated
# SPDX-License-Identifier: Apache-2.0
#
# TI Zephyr SDK documentation build configuration.
# Build: sphinx-build -b html doc/ doc/_build/html
# Reference: https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import sys
from pathlib import Path

TI_ZEPHYR_BASE = Path(__file__).resolve().parents[1]

# -- Project ------------------------------------------------------------------

project   = "TI Zephyr SDK"
copyright = "2026 Texas Instruments Incorporated"
author    = "Texas Instruments Zephyr SDK Team"
version   = "v1.0.0"
release   = "v1.0.0"

# -- Extensions ---------------------------------------------------------------

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.intersphinx",
    "sphinx.ext.todo",
]

# Cross-reference Zephyr docs (adjust path to built Zephyr docs if available)
intersphinx_mapping = {
    "zephyr": ("https://docs.zephyrproject.org/latest/", None),
}

# -- Options ------------------------------------------------------------------

source_suffix   = ".rst"
master_doc      = "index"
language        = "en"
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

# -- HTML output --------------------------------------------------------------

html_theme   = "sphinx_rtd_theme"
html_title   = f"TI Zephyr SDK {version}"
html_logo    = None
html_favicon = None

html_theme_options = {
    "navigation_depth": 4,
    "collapse_navigation": False,
}

# -- LaTeX output (for TI user guide PDF) ------------------------------------

latex_elements = {
    "papersize": "letterpaper",
    "pointsize":  "10pt",
}

latex_documents = [
    (master_doc, "ti-zephyr-sdk.tex", "TI Zephyr SDK User Guide",
     "Texas Instruments Incorporated", "manual"),
]
