# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_xcb_record INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_xcb_record ALIAS external_xcb_record)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XCB_RECORD REQUIRED IMPORTED_TARGET xcb-record)
    target_link_libraries(external_xcb_record INTERFACE PkgConfig::XCB_RECORD)
else()
    target_link_static_libraries(external_xcb_record INTERFACE xcb-record)
endif()
