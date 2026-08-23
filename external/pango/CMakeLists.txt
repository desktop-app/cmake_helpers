# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_pango INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_pango ALIAS external_pango)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DESKTOP_APP_PANGOCAIRO REQUIRED IMPORTED_TARGET pangocairo)
pkg_check_modules(DESKTOP_APP_PANGOFT2 IMPORTED_TARGET pangoft2)

target_link_libraries(external_pango
INTERFACE
    PkgConfig::DESKTOP_APP_PANGOCAIRO
    $<TARGET_NAME_IF_EXISTS:PkgConfig::DESKTOP_APP_PANGOFT2>
)
