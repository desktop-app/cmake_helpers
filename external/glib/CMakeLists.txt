# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_glib INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_glib ALIAS external_glib)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(GOBJECT REQUIRED IMPORTED_TARGET gobject-2.0)
pkg_check_modules(GIO REQUIRED IMPORTED_TARGET gio-2.0)

if (DESKTOP_APP_USE_PACKAGED)
    target_link_libraries(external_glib
    INTERFACE
        PkgConfig::GIO
        PkgConfig::GOBJECT
        PkgConfig::GLIB2
    )
else()
    target_include_directories(external_glib
    INTERFACE
        ${GIO_INCLUDE_DIRS}
        ${GOBJECT_INCLUDE_DIRS}
        ${GLIB2_INCLUDE_DIRS}
    )

    target_link_libraries(external_glib
    INTERFACE
        gio-2.0
        gobject-2.0
        glib-2.0
    )
endif()
