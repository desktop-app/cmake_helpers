# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_xcb INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_xcb ALIAS external_xcb)

add_subdirectory(xcb_keysyms)
add_subdirectory(xcb_record)
add_subdirectory(xcb_screensaver)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DESKTOP_APP_XCB REQUIRED IMPORTED_TARGET xcb)
target_link_libraries(external_xcb INTERFACE PkgConfig::DESKTOP_APP_XCB)
