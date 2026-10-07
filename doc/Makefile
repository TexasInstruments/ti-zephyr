SPHINX_BUILD  ?= sphinx-build
SPHINXOPTS    ?=
SOURCEDIR      = .
BUILDBASE      = ../_build/doc

# Optional SoC family (see SOCS in conf.py), e.g. "make html SOC=mspm0".
# Each SoC builds into its own directory so doctrees are not shared.
SOC           ?=
export SOC
BUILDDIR       = $(BUILDBASE)$(if $(SOC),/$(SOC))

.PHONY: html linkcheck clean help

help:
	@echo "Available targets:"
	@echo "  html       Build HTML documentation"
	@echo "  linkcheck  Check all external links"
	@echo "  clean      Remove build output and generated source copies"
	@echo ""
	@echo "Variables:"
	@echo "  SOC=<soc>  Build for one SoC family (mspm0, mspm33, am13, am62l)"

html:
	$(SPHINX_BUILD) -b html $(SPHINXOPTS) -d $(BUILDDIR)/doctrees $(SOURCEDIR) $(BUILDDIR)/html
	@echo ""
	@echo "Build finished. Output is in $(BUILDDIR)/html/"

linkcheck:
	$(SPHINX_BUILD) -b linkcheck $(SPHINXOPTS) $(SOURCEDIR) $(BUILDDIR)/linkcheck

clean:
	rm -rf $(BUILDBASE)
	rm -rf samples/ tests/
