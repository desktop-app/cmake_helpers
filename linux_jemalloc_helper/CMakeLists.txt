# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(linux_jemalloc_helper OBJECT)
init_target(linux_jemalloc_helper "(external)")
add_library(desktop-app::linux_jemalloc_helper ALIAS linux_jemalloc_helper)

nice_target_sources(linux_jemalloc_helper ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    linux_jemalloc_helper.cpp
)

target_link_libraries(linux_jemalloc_helper
PRIVATE
    desktop-app::external_jemalloc
)
