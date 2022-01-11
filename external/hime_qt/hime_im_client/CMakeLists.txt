# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_hime_im_client INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_hime_im_client ALIAS external_hime_im_client)
    return()
endif()

add_library(external_hime_im_client STATIC)
add_library(desktop-app::external_hime_im_client ALIAS external_hime_im_client)
init_target(external_hime_im_client "(external)")

set(hime_loc ${third_party_loc}/hime)
set(hime_im_client_src ${hime_loc}/src/im-client)

nice_target_sources(external_hime_im_client ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    hime_im_client_helper.cpp
)

target_include_directories(external_hime_im_client
PUBLIC
    ${hime_im_client_src}
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(X11 REQUIRED x11)

target_include_directories(external_hime_im_client SYSTEM
PRIVATE
    ${X11_INCLUDE_DIRS}
)
