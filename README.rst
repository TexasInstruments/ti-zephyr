ti-zephyr
#########

TI's downstream Zephyr SDK: a west-managed module for TI silicon.

Repository layout
******************

.. list-table::
   :header-rows: 1

   * - Path
     - Purpose
   * - ``west.yml``
     - West manifest — pins ``zephyr`` and every TI-relevant module
   * - ``drivers/``
     - Out-of-tree drivers
   * - ``samples/``
     - Out-of-box samples for TI boards
   * - ``tests/``
     - ztest/twister test suites for TI boards
   * - ``dts/``, ``include/``, ``lib/``
     - Devicetree bindings, public headers, libraries
   * - ``applications/``
     - Reference application scaffolding
   * - ``cmake/``
     - Shared CMake extensions
   * - ``doc/``
     - Sphinx documentation sources
   * - ``modules/``
     - TI modules pending their own west projects
   * - ``scripts/``
     - Development tooling

Getting started
****************

.. code-block:: shell

   west init -l ti-zephyr
   west update
   export ZEPHYR_BASE=$PWD/zephyr
   west build -b lp_mspm0g3519 ti-zephyr/samples/hello_world

License
*******

BSD 3-Clause License — see the `LICENSE file <LICENSE>`_. One
reference-only third-party tool invocation is disclosed in
`NOTICE <NOTICE>`_.
