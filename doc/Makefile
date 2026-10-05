SPHINX_BUILD  ?= sphinx-build
SOURCEDIR      = .
BUILDDIR       = ../_build/doc

.PHONY: html linkcheck clean help

help:
	@echo "Available targets:"
	@echo "  html       Build HTML documentation"
	@echo "  linkcheck  Check all external links"
	@echo "  clean      Remove build output and generated source copies"

html:
	$(SPHINX_BUILD) -b html -d $(BUILDDIR)/doctrees $(SOURCEDIR) $(BUILDDIR)/html
	@echo ""
	@echo "Build finished. Output is in $(BUILDDIR)/html/"

linkcheck:
	$(SPHINX_BUILD) -b linkcheck $(SOURCEDIR) $(BUILDDIR)/linkcheck

clean:
	rm -rf $(BUILDDIR)
	rm -rf samples/ tests/
