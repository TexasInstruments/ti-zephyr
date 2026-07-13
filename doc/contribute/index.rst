.. Copyright (c) 2026 Texas Instruments Incorporated
.. SPDX-License-Identifier: Apache-2.0

.. _ti-zephyr-contributing:

Contributing
############

Branch Model
************

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Branch
     - Purpose
   * - ``main``
     - Stable. SDK users clone from here. Tagged for releases.
       Only ``next`` merges here (via PR).
   * - ``next``
     - Integration. All feature PRs merge here first.
       Regularly promoted to ``main`` after validation.
   * - ``feature/*``
     - Individual features/fixes. Branch from ``next``.

Feature Branch Workflow
***********************

.. code-block:: bash

   # Branch from next
   git clone https://github.com/TexasInstruments/ti-zephyr --branch next <dir>
   cd <dir>
   git checkout -b feature/my-feature

   # Set up SDK workspace
   python3 install.py

   # Work, build, test
   source ~/ti/env_setup.sh
   west build -b lp_mspm0g3519 <dir>/samples/basic/blinky

   # Commit (Signed-off-by required)
   git commit -s -m "feat(boards): add lp_mspm33c341a support"

   # Push and open PR targeting 'next'
   git push origin feature/my-feature

Commit Message Convention
*************************

.. code-block:: none

   <type>(<scope>): <short description>

   [optional body — explain WHY, not what]

   Signed-off-by: Your Name <email@ti.com>

**Types:** ``feat``, ``fix``, ``docs``, ``refactor``, ``test``, ``ci``, ``chore``

**Scopes:** ``boards``, ``drivers``, ``samples``, ``install``, ``west``,
``openocd``, ``tests``, ``dts``, ``doc``

The ``Signed-off-by`` line certifies the
`Developer Certificate of Origin (DCO) <https://developercertificate.org/>`_.
It is **required** on every commit.

Replacing Fork Placeholders
***************************

When TI provides an official downstream fork URL:

.. code-block:: bash

   # 1. Update the remote url-base in west.yml
   #    Change: url-base: https://github.com/vvnpais   # PLACEHOLDER_TI_ZEPHYR_FORK
   #    To:     url-base: https://github.com/TexasInstruments/zephyr

   # 2. Update the revision in scripts/sdk_versions.py
   #    TI_ZEPHYR_REVISION = "v4.4.0-ti.1"

   # 3. Uncomment the project entry in west.yml if applicable

   # 4. Test
   west update
   west build -b lp_mspm0g3519 ti-zephyr/samples/basic/blinky

   # 5. Check remaining placeholders
   grep -r "PLACEHOLDER_" ti-zephyr/

Upstreaming to Zephyr
*********************

When content in ``ti-zephyr/`` is ready for upstream Zephyr:

.. code-block:: bash

   # Move to upstream tree
   cp -r ti-zephyr/boards/ti/<board>/ zephyr/boards/ti/
   rm -rf ti-zephyr/boards/ti/<board>/

   # Commit with upstream-ready format
   git -C zephyr add boards/ti/<board>/
   git -C zephyr commit -s -m "boards: ti: add <board> LaunchPad"

   # Push to TI zephyr fork, open PR to zephyrproject-rtos/zephyr

Same pattern for ``drivers/``, ``dts/``, ``samples/``, ``lib/``.

Code Style
**********

Follow `Zephyr coding style <https://docs.zephyrproject.org/latest/contribute/guidelines.html>`_:

* C: clang-format (``clang-format -style=file``)
* Python: PEP 8, no type annotations required
* RST/Markdown: 80-column wrap where practical

See Also
********

* :ref:`ti-zephyr-sdk` — SDK overview
* `CONTRIBUTING.md <https://github.com/TexasInstruments/ti-zephyr/blob/main/CONTRIBUTING.md>`_
* `Zephyr Contributing Guide <https://docs.zephyrproject.org/latest/contribute/index.html>`_
