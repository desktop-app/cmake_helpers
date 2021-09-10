# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(linux_glib_helper STATIC)
add_library(desktop-app::linux_glib_helper ALIAS linux_glib_helper)

nice_target_sources(linux_glib_helper ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    linux_glib_helper.cpp
)

target_link_libraries(linux_glib_helper
PUBLIC
    ${CMAKE_DL_LIBS}
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)

target_include_directories(linux_glib_helper SYSTEM
PRIVATE
    ${GLIB2_INCLUDE_DIRS}
    ${GOBJECT_INCLUDE_DIRS}
)
