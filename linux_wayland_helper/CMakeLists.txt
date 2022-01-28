# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(linux_wayland_helper STATIC)
init_target(linux_wayland_helper "(external)")
add_library(desktop-app::linux_wayland_helper ALIAS linux_wayland_helper)

nice_target_sources(linux_wayland_helper ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    linux_wayland_helper.cpp
)

target_link_libraries(linux_wayland_helper
PRIVATE
    desktop-app::external_gsl
)
