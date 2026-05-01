# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_microtex INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_microtex ALIAS external_microtex)

# MicroTeX is currently only used through the desktop-app fork (which
# carries the Qt setPixelSize fix, the \mathop limits-mode fix, and the
# tinyxml2 strip). No system-package fallback -- this fork is required.
add_library(external_microtex_bundled STATIC)
init_target(external_microtex_bundled "(external)")

set(microtex_loc ${third_party_loc}/MicroTeX)
set(microtex_src ${microtex_loc}/src)

target_sources(external_microtex_bundled
PRIVATE
    ${microtex_src}/atom/atom_basic.cpp
    ${microtex_src}/atom/atom_char.cpp
    ${microtex_src}/atom/atom_impl.cpp
    ${microtex_src}/atom/atom_matrix.cpp
    ${microtex_src}/atom/atom_row.cpp
    ${microtex_src}/atom/atom_space.cpp
    ${microtex_src}/atom/colors_def.cpp
    ${microtex_src}/atom/unit_conversion.cpp
    ${microtex_src}/box/box.cpp
    ${microtex_src}/box/box_factory.cpp
    ${microtex_src}/box/box_group.cpp
    ${microtex_src}/box/box_single.cpp
    ${microtex_src}/core/core.cpp
    ${microtex_src}/core/formula.cpp
    ${microtex_src}/core/formula_def.cpp
    ${microtex_src}/core/glue.cpp
    ${microtex_src}/core/localized_num.cpp
    ${microtex_src}/core/macro.cpp
    ${microtex_src}/core/macro_def.cpp
    ${microtex_src}/core/macro_impl.cpp
    ${microtex_src}/core/parser.cpp
    ${microtex_src}/fonts/alphabet.cpp
    ${microtex_src}/fonts/font_basic.cpp
    ${microtex_src}/fonts/font_info.cpp
    ${microtex_src}/fonts/fonts.cpp
    ${microtex_src}/utils/string_utils.cpp
    ${microtex_src}/utils/utf.cpp
    ${microtex_src}/utils/utils.cpp
    ${microtex_src}/res/builtin/formula_mappings.res.cpp
    ${microtex_src}/res/builtin/symbol_mapping.res.cpp
    ${microtex_src}/res/builtin/tex_param.res.cpp
    ${microtex_src}/res/builtin/tex_symbols.res.cpp
    ${microtex_src}/res/font/bi10.def.cpp
    ${microtex_src}/res/font/bx10.def.cpp
    ${microtex_src}/res/font/cmbsy10.def.cpp
    ${microtex_src}/res/font/cmbx10.def.cpp
    ${microtex_src}/res/font/cmbxti10.def.cpp
    ${microtex_src}/res/font/cmex10.def.cpp
    ${microtex_src}/res/font/cmmi10.def.cpp
    ${microtex_src}/res/font/cmmi10_unchanged.def.cpp
    ${microtex_src}/res/font/cmmib10.def.cpp
    ${microtex_src}/res/font/cmmib10_unchanged.def.cpp
    ${microtex_src}/res/font/cmr10.def.cpp
    ${microtex_src}/res/font/cmss10.def.cpp
    ${microtex_src}/res/font/cmssbx10.def.cpp
    ${microtex_src}/res/font/cmssi10.def.cpp
    ${microtex_src}/res/font/cmsy10.def.cpp
    ${microtex_src}/res/font/cmti10.def.cpp
    ${microtex_src}/res/font/cmti10_unchanged.def.cpp
    ${microtex_src}/res/font/cmtt10.def.cpp
    ${microtex_src}/res/font/dsrom10.def.cpp
    ${microtex_src}/res/font/eufb10.def.cpp
    ${microtex_src}/res/font/eufm10.def.cpp
    ${microtex_src}/res/font/i10.def.cpp
    ${microtex_src}/res/font/moustache.def.cpp
    ${microtex_src}/res/font/msam10.def.cpp
    ${microtex_src}/res/font/msbm10.def.cpp
    ${microtex_src}/res/font/r10.def.cpp
    ${microtex_src}/res/font/r10_unchanged.def.cpp
    ${microtex_src}/res/font/rsfs10.def.cpp
    ${microtex_src}/res/font/sb10.def.cpp
    ${microtex_src}/res/font/sbi10.def.cpp
    ${microtex_src}/res/font/si10.def.cpp
    ${microtex_src}/res/font/special.def.cpp
    ${microtex_src}/res/font/ss10.def.cpp
    ${microtex_src}/res/font/stmary10.def.cpp
    ${microtex_src}/res/font/tt10.def.cpp
    ${microtex_src}/res/reg/builtin_font_reg.cpp
    ${microtex_src}/res/reg/builtin_syms_reg.cpp
    ${microtex_src}/res/sym/amsfonts.def.cpp
    ${microtex_src}/res/sym/amssymb.def.cpp
    ${microtex_src}/res/sym/base.def.cpp
    ${microtex_src}/res/sym/stmaryrd.def.cpp
    ${microtex_src}/res/sym/symspecial.def.cpp
    ${microtex_src}/latex.cpp
    ${microtex_src}/render.cpp
    ${microtex_src}/platform/qt/graphic_qt.cpp
)

target_include_directories(external_microtex_bundled SYSTEM
PUBLIC
    ${microtex_src}
)

# BUILD_QT selects the Qt graphics backend in MicroTeX's config.h gates.
# Must be PUBLIC because it's checked from public headers (graphic_qt.h).
target_compile_definitions(external_microtex_bundled
PUBLIC
    BUILD_QT
)

# _USE_MATH_DEFINES exposes M_PI etc. on MSVC; needed by jkqtcommon-style
# constants pulled in by jkqtpcsstools.cpp -- here we keep it private since
# only the .cpps reference these constants.
target_compile_definitions(external_microtex_bundled
PRIVATE
    _USE_MATH_DEFINES
)

target_link_libraries(external_microtex_bundled
PUBLIC
    desktop-app::external_qt
)

# Bundle the TeX fonts that DEF_FONT(...) entries reference at runtime
# into Qt resources under prefix /res. Combined with the default
# RES_BASE = "res" and the Font_qt(file, size) ":/" prepend in
# src/platform/qt/graphic_qt.cpp, font lookups resolve to ":/res/..."
# without any disk dependency. Consumers must call:
#     Q_INIT_RESOURCE(bundled);
#     tex::LaTeX::initBundled();
# at startup. Q_INIT_RESOURCE is required because the resource-init
# translation unit lives inside a static library; without an explicit
# reference the linker may drop it.
target_sources(external_microtex_bundled PRIVATE ${microtex_loc}/res/bundled.qrc)
target_prepare_qrc(external_microtex_bundled)

# Tame upstream warnings; this is third-party code under MIT and we
# don't want /W4 noise drowning real diagnostics. The flags MUST be
# delivered via an INTERFACE library linked AFTER common_options, so
# their INTERFACE_COMPILE_OPTIONS land last on the compile command line
# (target's own PRIVATE COMPILE_OPTIONS would be ordered BEFORE the
# linked-library INTERFACE_COMPILE_OPTIONS by CMake, letting common_options'
# /W4 /WX silently win).
add_library(external_microtex_warnings_off INTERFACE)
if (MSVC)
    target_compile_options(external_microtex_warnings_off
    INTERFACE
        /W0
        /WX-
    )
else()
    target_compile_options(external_microtex_warnings_off
    INTERFACE
        -w
        -Wno-error
    )
endif()
target_link_libraries(external_microtex_bundled
PRIVATE
    external_microtex_warnings_off
)

target_link_libraries(external_microtex
INTERFACE
    external_microtex_bundled
)
