# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_glib INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_glib ALIAS external_glib)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DESKTOP_APP_GLIB2 REQUIRED IMPORTED_TARGET glib-2.0 gobject-2.0 gio-2.0 gio-unix-2.0)

target_link_libraries(external_glib
INTERFACE
    PkgConfig::DESKTOP_APP_GLIB2
)

target_compile_definitions(external_glib
INTERFACE
    GLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_56
    GLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_56
)

block()
    set(BUILD_TESTING OFF)
    set(BUILD_DOC OFF)
    set(BUILD_EXAMPLES OFF)
    add_subdirectory(cppgir EXCLUDE_FROM_ALL)
endblock()

include(generate_cppgir.cmake)

if (DESKTOP_APP_GLIB2_gio-unix-2.0_VERSION VERSION_GREATER_EQUAL 2.86)
    generate_cppgir(external_glib GioUnix-2.0)
else()
    generate_cppgir(external_glib Gio-2.0)
endif()
