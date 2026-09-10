# ------------------------------------------------------------------------------
# Makefile for ti-zephyr documentation build
#
# Copyright (c) 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: BSD-3-Clause
# ------------------------------------------------------------------------------

BUILDDIR ?= _build
DOC_TAG ?= development
SPHINXOPTS ?= -j auto -W --keep-going -T
SPHINXOPTS_EXTRA ?=

.PHONY: configure clean html doxygen linkcheck

html linkcheck doxygen: configure
	cmake --build ${BUILDDIR} --target $@

configure:
	cmake \
		-GNinja \
		-B${BUILDDIR} \
		-S. \
		-DDOC_TAG=${DOC_TAG} \
		-DSPHINXOPTS="${SPHINXOPTS}" \
		-DSPHINXOPTS_EXTRA="${SPHINXOPTS_EXTRA}"

clean:
	cmake --build ${BUILDDIR} --target clean
