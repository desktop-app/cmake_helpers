# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(linux_xcb_helper STATIC)
init_target(linux_xcb_helper "(external)")
add_library(desktop-app::linux_xcb_helper ALIAS linux_xcb_helper)

nice_target_sources(linux_xcb_helper ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    linux_xcb_helper.cpp
)
