# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_wayland_client INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_wayland_client ALIAS external_wayland_client)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WAYLAND_CLIENT REQUIRED IMPORTED_TARGET wayland-client)
    target_link_libraries(external_wayland_client INTERFACE PkgConfig::WAYLAND_CLIENT)
else()
    target_link_libraries(external_wayland_client INTERFACE desktop-app::linux_wayland_helper)
endif()
