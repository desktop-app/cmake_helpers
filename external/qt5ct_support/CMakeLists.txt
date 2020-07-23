# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt5ct_support INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_qt5ct_support ALIAS external_qt5ct_support)

add_subdirectory(qt5ct)
add_subdirectory(qt5ct_qtplugin)
add_subdirectory(qt5ct_style)

target_link_libraries(external_qt5ct_support
INTERFACE
    desktop-app::external_qt5ct_style
    desktop-app::external_qt5ct_qtplugin
)
