.. _ti-zephyr-contribute:

Contributing
############

Contributions of new samples and tests are welcome. This page covers the
mechanics of adding new content; for general Zephyr contribution guidelines
see the `Zephyr contribution guide
<https://docs.zephyrproject.org/latest/contribute/index.html>`_.

Adding a New Sample
*******************

1. **Create the sample directory** under ``samples/<category>/<sample_name>/``:

   .. code-block:: text

      samples/drivers/uart/my_sample/
      ├── src/
      │   └── main.c
      ├── CMakeLists.txt
      ├── prj.conf
      ├── tests.yaml
      └── README.rst

2. **Write the CMakeLists.txt** following the standard Zephyr pattern:

   .. code-block:: cmake

      cmake_minimum_required(VERSION 3.20.0)
      find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
      project(my_sample)
      target_sources(app PRIVATE src/main.c)

3. **Write README.rst** next to the source code. Use this structure:

   .. code-block:: rst

      .. _sample-my-sample:

      My Sample Title
      ###############

      Overview
      ********
      Brief description of what the sample demonstrates.

      Building and Running
      ********************

      .. code-block:: console

         west build -p always -b lp_mspm0g3507 ti-zephyr/samples/drivers/uart/my_sample
         west flash

      Sample Output
      *************

      .. code-block:: console

         <expected terminal output>

4. **Register the sample in the toctree.** Add an entry to the nearest
   category index file (e.g. ``samples/drivers/uart/uart.rst``). If no
   category index exists yet, create one and add it to its parent index.

5. **Verify the docs build** before submitting:

   .. code-block:: console

      cd ti-zephyr/doc
      make html

RST Style Conventions
*********************

Follow the same conventions as upstream Zephyr:

- Page title underlined with ``#`` (overline + underline)
- Section headings: ``*`` then ``=`` then ``-`` then ``~``
- All ``.. code-block::`` directives include a language tag (``console``, ``c``,
  ``cmake``, ``yaml``, etc.)
- Cross-reference labels (``.. _label:``) placed immediately before the
  section title they label, with a blank line between them

Adding a New Test
*****************

Follow the same directory structure as existing tests under ``tests/``. Refer
to :ref:`ti-zephyr-tests` for the ``tests.yaml`` format and how to run tests
with Twister.
