# Copyright (c) 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.20.0)
project(TI-Zephyr-Doc LANGUAGES)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE} ../zephyr)

file(TO_CMAKE_PATH "${CMAKE_CURRENT_LIST_DIR}/.." TI_ZEPHYR_BASE)
message(STATUS "ti-zephyr base: ${TI_ZEPHYR_BASE}")

#-------------------------------------------------------------------------------
# Options

set(SPHINXOPTS "-j auto -W --keep-going -T" CACHE STRING "Default Sphinx Options")
set(SPHINXOPTS_EXTRA "" CACHE STRING "Extra Sphinx Options (added to defaults)")
set(DOC_TAG "development" CACHE STRING "Documentation tag")

separate_arguments(SPHINXOPTS)
separate_arguments(SPHINXOPTS_EXTRA)

#-------------------------------------------------------------------------------
# Dependencies

find_package(Doxygen REQUIRED dot)

find_program(SPHINXBUILD sphinx-build)
if(NOT SPHINXBUILD)
  message(WARNING "The 'sphinx-build' command was not found; Sphinx-based targets will not be available.")
endif()
find_program(SPHINXAUTOBUILD sphinx-autobuild)

#-------------------------------------------------------------------------------
# Environment & Paths

set(SPHINX_ENV
  DOXYGEN_EXECUTABLE=${DOXYGEN_EXECUTABLE}
  DOT_EXECUTABLE=${DOXYGEN_DOT_EXECUTABLE}
  DOCS_HTML_DIR=${CMAKE_CURRENT_BINARY_DIR}/html
)

set(DOCS_CFG_DIR ${CMAKE_CURRENT_LIST_DIR})
set(DOCS_DOCTREE_DIR ${CMAKE_CURRENT_BINARY_DIR}/doctrees)
set(DOCS_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(DOCS_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
set(DOCS_HTML_DIR ${CMAKE_CURRENT_BINARY_DIR}/html)
set(DOCS_LINKCHECK_DIR ${CMAKE_CURRENT_BINARY_DIR}/linkcheck)

#-------------------------------------------------------------------------------
# Functions

function(add_doc_target name)
  add_custom_target(${name} ${ARGN})
  add_custom_target(${name}-nodeps ${ARGN})
endfunction()

#-------------------------------------------------------------------------------
# Doxygen (standalone)

set(DOXY_OUT ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYFILE_IN ${CMAKE_CURRENT_LIST_DIR}/ti-zephyr.doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/ti-zephyr.doxyfile)
set(TI_ZEPHYR_VERSION "${PROJECT_VERSION}")

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

add_custom_target(
  doxygen
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
  COMMENT "Running Doxygen..."
)

set_target_properties(
  doxygen
  PROPERTIES ADDITIONAL_CLEAN_FILES "${DOXY_OUT}"
)

#-------------------------------------------------------------------------------
# html

set(SPHINX_TAGS "${DOC_TAG}")
set(SPHINX_TAGS_ARGS "")
foreach(tag ${SPHINX_TAGS})
  list(APPEND SPHINX_TAGS_ARGS "-t" "${tag}")
endforeach()

if(SPHINXBUILD)
  add_doc_target(
    html
    COMMAND ${CMAKE_COMMAND} -E env ${SPHINX_ENV} OUTPUT_DIR=${DOCS_HTML_DIR}
    ${SPHINXBUILD}
      -b html
      -c ${DOCS_CFG_DIR}
      -d ${DOCS_DOCTREE_DIR}
      -w ${DOCS_BUILD_DIR}/html.log
      ${SPHINX_TAGS_ARGS}
      ${SPHINXOPTS}
      ${SPHINXOPTS_EXTRA}
      ${DOCS_SRC_DIR}
      ${DOCS_HTML_DIR}
    USES_TERMINAL
    COMMENT "Running Sphinx HTML build..."
  )

  set_target_properties(
    html html-nodeps
    PROPERTIES ADDITIONAL_CLEAN_FILES "${DOCS_HTML_DIR};${DOCS_DOCTREE_DIR}"
  )
endif()

#-------------------------------------------------------------------------------
# linkcheck

if(SPHINXBUILD)
  add_doc_target(
    linkcheck
    COMMAND ${CMAKE_COMMAND} -E env ${SPHINX_ENV} OUTPUT_DIR=${DOCS_LINKCHECK_DIR}
    ${SPHINXBUILD}
      -b linkcheck
      -c ${DOCS_CFG_DIR}
      -d ${DOCS_DOCTREE_DIR}
      -w ${DOCS_BUILD_DIR}/linkcheck.log
      ${SPHINX_TAGS_ARGS}
      ${SPHINXOPTS}
      ${SPHINXOPTS_EXTRA}
      ${DOCS_SRC_DIR}
      ${DOCS_LINKCHECK_DIR}
    USES_TERMINAL
    COMMENT "Running Sphinx link check..."
  )

  set_target_properties(
    linkcheck linkcheck-nodeps
    PROPERTIES ADDITIONAL_CLEAN_FILES "${DOCS_LINKCHECK_DIR};${DOCS_DOCTREE_DIR}"
  )
endif()
