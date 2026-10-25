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

.PHONY: html linkcheck clean help check-soc

help:
	@echo "Usage: make <target> SOC=<soc>"
	@echo ""
	@echo "Targets:"
	@echo "  html       Build HTML documentation for one SoC family"
	@echo "  linkcheck  Check all external links for one SoC family"
	@echo "  clean      Remove build output and generated source copies"
	@echo ""
	@echo "SOC (required for html and linkcheck): $(SOCS)"

check-soc:
ifeq ($(filter $(SOC),$(SOCS)),)
	$(error SOC is required and must be one of: $(SOCS). Example: make html SOC=$(firstword $(SOCS)))
endif

html:
ifeq ($(SOC),)
	@echo "No SOC specified — building for all: $(SOCS)"
	@for s in $(SOCS); do $(MAKE) html SOC=$$s || exit 1; done
else
	$(MAKE) check-soc html-target
endif

html-target: check-soc
	$(SPHINX_BUILD) -b html $(SPHINXOPTS) -d $(BUILDDIR)/doctrees $(SOURCEDIR) $(HTMLOUTDIR)
	@echo ""
	@echo "Build finished. Output is in $(HTMLOUTDIR)"

linkcheck: check-soc
	$(SPHINX_BUILD) -b linkcheck $(SPHINXOPTS) -d $(BUILDDIR)/doctrees $(SOURCEDIR) $(BUILDDIR)/linkcheck

clean:
	rm -rf $(BUILDBASE)
	rm -rf samples/ tests/
	if [ -n "$(SOC)" ]; then rm -rf ./html/$(SOC); else for s in $(SOCS); do rm -rf ./html/$$s; done; fi
