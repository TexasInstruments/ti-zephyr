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
           └── testcase.yaml

The ``testcase.yaml`` file defines one or more test scenarios, board filters,
and harness configuration. See the
`Zephyr testcase.yaml reference
<https://docs.zephyrproject.org/latest/develop/test/ztest.html>`_
for the full schema. Samples use ``sample.yaml`` instead; Twister in this
Zephyr version only discovers tests by those two file names.

Running Tests with Twister
**************************

Zephyr's ``west twister`` picks up the tests, samples and applications of this
repository when ``-T`` points at it: twister scans the given path recursively
for ``testcase.yaml`` and ``sample.yaml`` files. Run the commands below from the
``zephyr`` directory of the west workspace, where ``../ti-zephyr`` is the
path to this repository.

Run all TI Zephyr tests, samples and applications on a specific board (see the
|soc_board_support| for the board IDs):

.. code-block:: console

   west twister -T ../ti-zephyr -p <board>

Build only, without flashing or running on hardware:

.. code-block:: console

   west twister -T ../ti-zephyr -p <board> --build-only

Run a single test or sample by its directory:

.. code-block:: console

   west twister -T ../ti-zephyr/tests/<subsystem>/<test_name> -p <board>
   west twister -T ../ti-zephyr/samples/<subsystem>/<sample_name> -p <board>

A test can also be selected by scenario name instead of path
(``west twister -T ../ti-zephyr --list-tests`` shows the names):

.. code-block:: console

   west twister -T ../ti-zephyr -s <scenario> -p <board>

Filter by tag:

.. code-block:: console

   west twister -T ../ti-zephyr --tag drivers -p <board>

Save the selected tests as a test plan and run it later:

.. code-block:: console

   west twister -T ../ti-zephyr -p <board> --save-tests plan.json
   west twister --load-tests plan.json

Building and Running Applications
*********************************

The reference applications under ``applications/`` are fully operational and
are handled by twister like any other sample. Each application has a
``sample.yaml`` that defines an ``application.<name>`` scenario, so the
commands above already include them.

Build and run all applications for a board:

.. code-block:: console

   west twister -T ../ti-zephyr/applications -p <board>

Build and run a single application:

.. code-block:: console

   west twister -T ../ti-zephyr/applications/<application> -p <board>

Add ``--build-only`` to build without flashing or running on hardware. To
flash and run on a connected board, use twister's device testing options
(``--device-testing`` with a hardware map), or build the application and flash
it directly with ``west flash``.

The applications set ``harness: none`` in their ``sample.yaml``, so they have
no console output check. Verify their behavior on the board itself, for
example the display and touch response.

Applications that need a shield, which supplies the display and touch
devicetree nodes, set it in the ``extra_args`` of their ``sample.yaml``, for
example ``extra_args: SHIELD=<shield>``. Twister has no command-line option
for shields. The build output is written to the ``twister-out`` directory,
or to the directory given with ``--outdir``. See the ``README.rst`` of each
application for the supported board, wiring and how to flash it.
