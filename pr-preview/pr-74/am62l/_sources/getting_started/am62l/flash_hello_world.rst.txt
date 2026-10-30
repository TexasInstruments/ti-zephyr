Step 4: Flash Hello World Example
#################################

AM62L runs the application from the SD card / USB-DFU prepared in Step 2.

Generated Boot Files
====================

A default build generates:

.. code-block:: console

   build/zephyr/zephyr.bin     # Zephyr application binary
   build/zephyr/tiboot3.bin    # Stage 1 bootloader (BL1 + TIFS + board configs)
   build/zephyr/bl1.bin        # TF-A BL1 binary (intermediate)
   build/zephyr/tispl.bin      # Stage 2 boot image (TF-A BL31 + Zephyr)
   build/zephyr/bl31.bin       # TF-A BL31 binary (intermediate)

Booting from SD Card
********************

The AM62L EVM supports two boot methods: direct boot (no U-Boot) and U-Boot-based boot.

Option 1: Direct Boot (No U-Boot)
==================================

This method boots Zephyr directly using only the generated boot images.

**Preparation:**

1. Prepare an SD card with a FAT32 boot partition
2. Copy the generated boot images to the SD card:

   .. code-block:: console

      cp build/zephyr/tiboot3.bin /media/boot/
      cp build/zephyr/tispl.bin /media/boot/

3. Insert the SD card and power on the board

**Boot Sequence:**

.. code-block:: text

   Boot ROM → tiboot3.bin (BL1 + TIFS) → tispl.bin (TF-A + Zephyr)
                                                           ↓
                                                    Zephyr Running

The boot ROM loads ``tiboot3.bin``, which initializes the hardware and loads
``tispl.bin`` into DDR. The TF-A BL31 then starts the Zephyr application at
0x82000000. No U-Boot is involved.

Option 2: U-Boot-Based Boot
============================

Copy the compiled ``zephyr.bin`` to the first FAT partition of the SD card and
plug the SD card into the board. Power it up and stop the u-boot execution at
prompt.

Use U-Boot to load and start zephyr.bin:

.. code-block:: console

   fatload mmc 1:1 0x82000000 zephyr.bin; dcache flush; icache flush; dcache off; icache off; go 0x82000000

The Zephyr application should start running on the A53 core.

Verify
******

Open a serial terminal at 115200 8N1. Expected output:

.. code-block:: console

   *** Booting Zephyr OS build v4.4.0-ti ***
   Hello World! <board>
