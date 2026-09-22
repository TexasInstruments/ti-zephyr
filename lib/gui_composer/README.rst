GUI Composer middleware
########################

Scope
*****

Port of TI's GUI Composer PC-tool communication middleware: a
transport-agnostic RX-dispatch/TX protocol layer speaking either
MessagePack (mpack) or JSON over a byte stream, plus a Zephyr UART
backend implementing the transport contract
(:file:`include/ti/gui_composer/transport.h`).

Applications call ``gui_init()`` / ``gui_init_rx_cmd()`` to bring up the
protocol and register RX command handlers, and ``GUIComm_send*()`` to
transmit named key/value pairs the GUI Composer PC application (or any
peer speaking the same wire protocol) can decode. See
:zephyr:code-sample:`simple_gui_demo` for the smallest working
example, and :zephyr:code-sample:`gui_composer_demo` for a
sensor/actuator-backed one.

Integration
***********

Enable with :kconfig:option:`CONFIG_GUI_COMPOSER`, select exactly one
protocol via the ``GUI_COMPOSER_PROTOCOL`` choice
(:kconfig:option:`CONFIG_GUI_COMPOSER_PROTOCOL_MSGPACK` or
:kconfig:option:`CONFIG_GUI_COMPOSER_PROTOCOL_JSON`), and add a
``/chosen/zephyr,gui-composer-uart`` devicetree node pointing at the
UART to carry the protocol -- see either sample's board overlay for
the pattern.

Only one protocol may be linked per image: ``gui_json.c`` and
``gui_mpack.c`` define identically-named public symbols with
incompatible RX callback signatures, matching how upstream TI examples
are never linked together either.

Owner
*****

Texas Instruments Incorporated.

Licensing
*********

TI-authored files (``gui_json.*``, ``gui_mpack.*``, ``guicomm_json.*``,
``guicomm_mpack.*``, :file:`src/backends/transport.c`,
:file:`include/ti/gui_composer/transport.h`,
:file:`include/gui_composer/gui_composer.h`, ``Kconfig``,
``CMakeLists.txt``) are BSD-3-Clause.

Two third-party libraries are bundled verbatim under a different
license, each carrying its own SPDX header:

- :file:`src/mpack/` -- `mpack <https://github.com/ludocode/mpack>`_,
  MIT License, Copyright (c) 2015-2018 Nicholas Fraser.
- :file:`include/ti/gui_composer/jsmn/` -- `jsmn
  <https://github.com/zserge/jsmn>`_, MIT License, Copyright (c) 2010
  Serge A. Zaitsev.
