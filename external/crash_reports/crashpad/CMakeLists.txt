# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_crashpad INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_crashpad ALIAS external_crashpad)

target_include_directories(external_crashpad SYSTEM
INTERFACE
    ${libs_loc}/crashpad
    ${libs_loc}/crashpad/third_party/mini_chromium/mini_chromium
)

set(crashpad_lib_loc ${libs_loc}/crashpad/out/$<IF:$<CONFIG:Debug>,Debug,Release>)

target_link_libraries(external_crashpad
INTERFACE
    ${crashpad_lib_loc}/libbase.a
    ${crashpad_lib_loc}/libcrashpad_client.a
    ${crashpad_lib_loc}/libcrashpad_util.a
    bsm
)
