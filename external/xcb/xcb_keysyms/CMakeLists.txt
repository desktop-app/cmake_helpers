# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_xcb_keysyms INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_xcb_keysyms ALIAS external_xcb_keysyms)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XCB_KEYSYMS REQUIRED IMPORTED_TARGET xcb-keysyms)
    target_link_libraries(external_xcb_keysyms INTERFACE PkgConfig::XCB_KEYSYMS)
else()
    target_link_static_libraries(external_xcb_keysyms INTERFACE xcb-keysyms)
endif()
