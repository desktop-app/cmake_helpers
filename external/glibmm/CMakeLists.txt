# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_glibmm INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_glibmm ALIAS external_glibmm)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIBMM REQUIRED IMPORTED_TARGET glibmm-2.4)
pkg_check_modules(GIOMM REQUIRED IMPORTED_TARGET giomm-2.4)

if (DESKTOP_APP_USE_PACKAGED)
    target_link_libraries(external_glibmm
    INTERFACE
        PkgConfig::GIOMM
        PkgConfig::GLIBMM
    )
else()
    target_include_directories(external_glibmm SYSTEM
    INTERFACE
        ${GIOMM_INCLUDE_DIRS}
        ${GLIBMM_INCLUDE_DIRS}
    )

    target_link_static_libraries(external_glibmm
    INTERFACE
        giomm-2.4
        glibmm-2.4
        sigc-2.0
    )

    target_link_libraries(external_glibmm
    INTERFACE
        gio-2.0
        gobject-2.0
        glib-2.0
    )
endif()
