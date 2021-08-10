# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(win_directx_helper STATIC)
init_target(win_directx_helper "(external)")
add_library(desktop-app::win_directx_helper ALIAS win_directx_helper)

nice_target_sources(win_directx_helper ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    win_directx_helper.cpp
)

validate_d3d_compiler(win_directx_helper)

target_link_libraries(win_directx_helper
PUBLIC
    desktop-app::external_openssl
)
