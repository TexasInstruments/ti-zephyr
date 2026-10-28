.. ti-socs: mspm0 mspm33 am13

.. _ti-zephyr-tool-openocd:

OpenOCD
#######

.. note::

   This page is a placeholder. More details will be added later.

`OpenOCD <https://openocd.org>`_ is the open-source on-chip debugger that
``west flash`` uses to program |soc_name| boards. It is included in the Zephyr
SDK installation.

Flash an application by running ``west flash`` from the ``zephyr`` folder and
pointing it at the SDK's OpenOCD:

   .. code-block:: shell

      west flash --openocd <openocd-dir>/src/openocd --openocd-search <openocd-dir>/tcl

See :doc:`/getting_started/index` for a complete build and flash walkthrough.
