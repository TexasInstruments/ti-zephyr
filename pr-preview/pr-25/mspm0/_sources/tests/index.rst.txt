.. _ti-zephyr-tests:

Tests
#####

TI Zephyr test suites live under ``tests/`` and are run with
`Twister <https://docs.zephyrproject.org/latest/develop/test/twister.html>`_,
Zephyr's standard test runner.

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

Run all TI Zephyr tests on a specific board:

.. code-block:: console

   cd workspace
   west twister -T ti-zephyr/tests --board lp_mspm0g3507

Run a single test suite:

.. code-block:: console

   west twister -T ti-zephyr/tests/drivers/pwm --board lp_mspm0g3507

Filter by tag:

.. code-block:: console

   west twister -T ti-zephyr/tests --tag drivers --board lp_mspm0g3507

Writing a New Test
******************

1. Create the directory structure under ``tests/<subsystem>/<test_name>/``.
2. Write the test in ``src/main.c`` using the
   `Ztest framework
   <https://docs.zephyrproject.org/latest/develop/test/ztest.html>`_.
3. Add a ``tests.yaml`` with at least one scenario entry whose ID follows
   the ``testing.<subsystem>.<test_name>`` dotted-namespace convention.
4. Add board overlays under ``boards/`` if hardware-specific pin configuration
   is needed.
