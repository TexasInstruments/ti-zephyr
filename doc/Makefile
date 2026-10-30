SPHINX_BUILD  ?= sphinx-build
SPHINXOPTS    ?=
SOURCEDIR      = .
BUILDBASE      = ../_build/doc

# SoC family to build (required for html/linkcheck), e.g. "make html SOC=mspm0".
# The valid values are the keys of socs.json. Each SoC builds into its own
# directory so doctrees are not shared.
SOC           ?=
export SOC
SOCS           = $(shell python3 -c "import json; print(' '.join(json.load(open('socs.json'))))")
BUILDDIR       = $(BUILDBASE)/$(SOC)
HTMLOUTDIR	   = ./html/$(SOC)

.DEFAULT_GOAL := html

# When ti-zephyr sits next to zephyr in a west workspace, "make html" also
# handles the main Zephyr docs and publishes both sites under <workspace>/docs/,
# so relative links from these docs into the Zephyr docs resolve:
#   <workspace>/docs/zephyr/            main Zephyr docs
#   <workspace>/docs/ti-zephyr/<soc>/   this site
#   <workspace>/docs/ti-zephyr/*-ZEPHYR-SDK_manifest.html
# Without a Zephyr tree (e.g. CI) the Zephyr part and the publishing are skipped.
# Without SOC, the Zephyr docs are handled once and then every SoC is built.
#
# Each build is skipped when nothing changed in its repository since the last
# successful build. Any file counts (not just docs), except generated output
# (see the *_PRUNE lists). A change is a file added, removed, resized or
# modified; no git is needed. The fingerprint is stored in a stamp file after
# a successful build only, so a failed build is retried next time.
#   ZEPHYR_DOCS  auto (default) | always | never
#                zephyr repo; "never" keeps the existing output and publishes it
#   TI_DOCS      auto (default) | always
#                ti-zephyr repo, tracked per SoC
# Needs GNU find (-printf).
ZEPHYR_DIR        ?= $(or $(ZEPHYR_BASE),../../zephyr)
ZEPHYR_HTML       ?= $(ZEPHYR_DIR)/doc/_build/html
PUBLISH_DIR       ?= $(ZEPHYR_DIR)/../docs
ZEPHYR_DOCS       ?= auto
# Zephyr's default adds -W, so any warning in the upstream docs fails the build.
ZEPHYR_SPHINXOPTS ?= -j auto --keep-going -T
ZEPHYR_DOCS_OPTS  ?= HW_FEATURES_VENDOR_FILTER=ti SPHINXOPTS="$(ZEPHYR_SPHINXOPTS)"
TI_DOCS           ?= auto
TI_DIR             = ..
ZEPHYR_STAMP       = $(BUILDBASE)/zephyr-docs.fingerprint
TI_STAMP           = $(BUILDDIR)/ti-docs.fingerprint
HAVE_ZEPHYR        = $(if $(wildcard $(ZEPHYR_DIR)/doc/Makefile),1,0)

ifeq ($(filter $(ZEPHYR_DOCS),auto always never),)
$(error ZEPHYR_DOCS must be auto, always or never (got '$(ZEPHYR_DOCS)'))
endif
ifeq ($(filter $(TI_DOCS),auto always),)
$(error TI_DOCS must be auto or always (got '$(TI_DOCS)'))
endif

# Not part of a fingerprint: VCS data, bytecode and generated output. Zephyr's
# own doc/build/ is source (documentation of the build system), so it is kept.
FP_PRUNE        = -name .git -o -name __pycache__ -o -name '*.pyc'
ZEPHYR_PRUNE    = -path '$(ZEPHYR_DIR)/doc/_build' -o -path '$(ZEPHYR_DIR)/twister-out*' \
                  -o -path '$(ZEPHYR_DIR)/scripts/pylib/twister/parsetab.py'
TI_PRUNE        = -path '$(TI_DIR)/_build' -o -path '$(TI_DIR)/build' -o -path '$(TI_DIR)/twister-out*' \
                  -o -path '$(TI_DIR)/doc/html' -o -path '$(TI_DIR)/doc/samples' -o -path '$(TI_DIR)/doc/tests' -o -path '$(TI_DIR)/doc/applications'

# $(call fingerprint,dir,prune-expr): hash of "path mtime size" for every file
# under dir, minus the pruned paths. Empty if dir does not exist.
fingerprint = $(if $(wildcard $(1)),$(shell find $(1) \( $(FP_PRUNE) -o $(2) \) -prune -o \( -type f -o -type l \) -printf '%P %T@ %s\n' 2>/dev/null | LC_ALL=C sort | sha1sum | cut -d' ' -f1))

# $(call needs_build,mode,fingerprint,stamp,output): non-empty if a build is
# needed. Not needed when mode is "never", or when the fingerprint equals the
# one in the stamp and the previous output still exists.
needs_build = $(if $(filter always,$(1)),1,$(if $(filter never,$(1)),,$(if $(and $(2),$(filter $(2),$(shell cat $(3) 2>/dev/null)),$(wildcard $(4))),,1)))

# Computed once, before anything is built, and only for the targets using them.
# "html" handles the Zephyr docs once; each SoC is then built by an "html-target"
# sub-make, which has its own SOC and therefore its own ti-zephyr fingerprint.
# MAKECMDGOALS is empty for a plain "make", so fall back to the default goal.
GOALS := $(or $(MAKECMDGOALS),$(.DEFAULT_GOAL))
ifneq ($(filter html zephyr-html,$(GOALS)),)
ZEPHYR_FP      := $(call fingerprint,$(ZEPHYR_DIR),$(ZEPHYR_PRUNE))
ZEPHYR_REBUILD := $(call needs_build,$(ZEPHYR_DOCS),$(ZEPHYR_FP),$(ZEPHYR_STAMP),$(ZEPHYR_HTML)/index.html)
endif
ifneq ($(filter html-target,$(GOALS)),)
TI_FP          := $(call fingerprint,$(TI_DIR),$(TI_PRUNE))
TI_REBUILD     := $(call needs_build,$(TI_DOCS),$(TI_FP),$(TI_STAMP),$(HTMLOUTDIR)/index.html)
endif

.PHONY: html linkcheck clean help check-soc zephyr-html

help:
	@echo "Usage: make <target> SOC=<soc>"
	@echo ""
	@echo "Targets:"
	@echo "  html         Build the main Zephyr docs, then the HTML documentation for one SoC"
	@echo "               family (every family if SOC is not given); publish both to $(PUBLISH_DIR)"
	@echo "  zephyr-html  Only the main Zephyr docs part of html"
	@echo "  linkcheck    Check all external links for one SoC family"
	@echo "  clean        Remove build output, fingerprints, html output and published copies"
	@echo ""
	@echo "SOC (required for linkcheck, optional for html): $(SOCS)"
	@echo ""
	@echo "A docs build is skipped when nothing changed in its repository since the"
	@echo "last successful build. Override with:"1
	@echo "  ZEPHYR_DOCS=always|never   main Zephyr docs (default: auto)"
	@echo "  TI_DOCS=always             ti-zephyr docs (default: auto)"

check-soc:
ifeq ($(filter $(SOC),$(SOCS)),)
	$(error SOC is required and must be one of: $(SOCS). Example: make html SOC=$(firstword $(SOCS)))
endif

zephyr-html: $(if $(SOC),check-soc)
ifeq ($(HAVE_ZEPHYR),1)
ifneq ($(ZEPHYR_REBUILD),)
	@echo "Zephyr changed or ZEPHYR_DOCS=always: building the main Zephyr docs"
	$(MAKE) -C $(ZEPHYR_DIR)/doc html $(ZEPHYR_DOCS_OPTS)
	mkdir -p $(BUILDBASE)
	echo '$(ZEPHYR_FP)' > $(ZEPHYR_STAMP)
else
	@echo "Not rebuilding the main Zephyr docs ($(if $(filter never,$(ZEPHYR_DOCS)),ZEPHYR_DOCS=never,zephyr unchanged since the last build))"
endif
	@if [ -f $(ZEPHYR_HTML)/index.html ]; then \
		mkdir -p $(PUBLISH_DIR)/zephyr && \
		rsync -a --delete $(ZEPHYR_HTML)/ $(PUBLISH_DIR)/zephyr/ && \
		echo "Zephyr docs copied to $(PUBLISH_DIR)/zephyr"; \
	else \
		echo "Warning: no Zephyr docs at $(ZEPHYR_HTML); links into them will not work"; \
	fi
else
	@echo "Skipping the main Zephyr docs (no Zephyr tree at $(ZEPHYR_DIR))"
endif

html: zephyr-html
ifeq ($(SOC),)
	@echo "No SOC specified — building for all: $(SOCS)"
	@for s in $(SOCS); do $(MAKE) html-target SOC=$$s || exit 1; done
else
	$(MAKE) check-soc html-target
endif

html-target: check-soc
ifneq ($(TI_REBUILD),)
	@echo "ti-zephyr changed or TI_DOCS=always: building the $(SOC) docs"
	$(SPHINX_BUILD) -b html $(SPHINXOPTS) -d $(BUILDDIR)/doctrees $(SOURCEDIR) $(HTMLOUTDIR)
	mkdir -p $(BUILDDIR)
	echo '$(TI_FP)' > $(TI_STAMP)
else
	@echo "Not rebuilding the $(SOC) docs (ti-zephyr unchanged since the last build)"
endif
ifeq ($(HAVE_ZEPHYR),1)
	mkdir -p $(PUBLISH_DIR)/ti-zephyr/$(SOC)
	rsync -a --delete $(HTMLOUTDIR)/ $(PUBLISH_DIR)/ti-zephyr/$(SOC)/
	cp html/*-ZEPHYR-SDK_manifest.html $(PUBLISH_DIR)/ti-zephyr/
	@echo "TI docs copied to $(PUBLISH_DIR)/ti-zephyr/$(SOC)"
endif
	@echo ""
	@echo "Output is in $(HTMLOUTDIR)"

linkcheck: check-soc
	$(SPHINX_BUILD) -b linkcheck $(SPHINXOPTS) -d $(BUILDDIR)/doctrees $(SOURCEDIR) $(BUILDDIR)/linkcheck

clean:
	rm -rf $(BUILDBASE)
	rm -rf samples/ tests/ applications/
	if [ -n "$(SOC)" ]; then rm -rf ./html/$(SOC); else for s in $(SOCS); do rm -rf ./html/$$s; done; fi
ifeq ($(HAVE_ZEPHYR),1)
	if [ -n "$(SOC)" ]; then rm -rf $(PUBLISH_DIR)/ti-zephyr/$(SOC); else rm -rf $(PUBLISH_DIR)/ti-zephyr $(PUBLISH_DIR)/zephyr; fi
endif
