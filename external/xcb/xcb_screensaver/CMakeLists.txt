# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_xcb_screensaver INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_xcb_screensaver ALIAS external_xcb_screensaver)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DESKTOP_APP_XCB_SCREENSAVER REQUIRED IMPORTED_TARGET xcb-screensaver)
target_link_libraries(external_xcb_screensaver INTERFACE PkgConfig::DESKTOP_APP_XCB_SCREENSAVER)
