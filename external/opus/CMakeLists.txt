# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_opus INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_opus ALIAS external_opus)

target_include_directories(external_opus SYSTEM
INTERFACE
    ${libs_loc}/opus/include
)

target_link_directories(external_opus
INTERFACE
    ${libs_loc}/opus/win32/VS2015/Win32/$<IF:$<CONFIG:Debug>,Debug,Release>
)

target_link_libraries(external_opus
INTERFACE
    opus
)
