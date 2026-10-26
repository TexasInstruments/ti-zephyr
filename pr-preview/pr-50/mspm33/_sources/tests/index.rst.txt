.. _ti-zephyr-tests:

Tests
#####

TI Zephyr test suites live under ``tests/`` and are run with
`Twister <https://docs.zephyrproject.org/latest/develop/test/twister.html>`_,
Zephyr's standard test runner. Only the tests that apply to |soc_name| are
listed.

.. toctree::
   :titlesonly:
   :maxdepth: 1
   :glob:

   */README

Test Structure
**************

Tests follow the same structure as upstream Zephyr tests:

.. code-block:: text

   tests/
   └── <subsystem>/
       └── <test_name>/
           ├── src/
           │   └── main.c
           ├── CMakeLists.txt
           ├── prj.conf
           └── tests.yaml

The ``tests.yaml`` file defines one or more test scenarios, board filters,
and harness configuration. See the
`Zephyr tests.yaml reference
<https://docs.zephyrproject.org/latest/develop/test/ztest.html>`_
for the full schema.

Running Tests with Twister
**************************

Run all TI Zephyr tests on a specific board (see the |soc_board_support| for the board IDs):

.. code-block:: console

   cd workspace
   west twister -T ti-zephyr/tests --board <board>

Run a single test suite:

.. code-block:: console

   west twister -T ti-zephyr/tests/<subsystem>/<test_name> --board <board>

Filter by tag:

.. code-block:: console

   west twister -T ti-zephyr/tests --tag drivers --board <board>
