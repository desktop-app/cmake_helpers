# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt_static_plugins_hime_im_client STATIC)
add_library(desktop-app::external_qt_static_plugins_hime_im_client ALIAS external_qt_static_plugins_hime_im_client)
init_target(external_qt_static_plugins_hime_im_client "(external)")

set(hime_loc ${third_party_loc}/hime)
set(hime_im_client_src ${hime_loc}/src/im-client)

nice_target_sources(external_qt_static_plugins_hime_im_client ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    hime_im_client_helper.cpp
)

target_include_directories(external_qt_static_plugins_hime_im_client
PUBLIC
    ${hime_im_client_src}
)

target_link_libraries(external_qt_static_plugins_hime_im_client
PRIVATE
    desktop-app::lib_base
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DESKTOP_APP_X11 REQUIRED x11)

target_include_directories(external_qt_static_plugins_hime_im_client SYSTEM
PRIVATE
    ${DESKTOP_APP_X11_INCLUDE_DIRS}
)
