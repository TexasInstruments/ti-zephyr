# SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
#
# SPDX-License-Identifier: LicenseRef-TI-TFL

add_subdirectory_ifdef(CONFIG_GUI_COMPOSER gui_composer)
add_subdirectory_ifdef(CONFIG_TI_MATHLIB_FASTRTS mathlib)
