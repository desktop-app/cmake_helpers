# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_breakpad INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_breakpad ALIAS external_breakpad)

target_include_directories(external_breakpad SYSTEM
INTERFACE
    ${libs_loc}/breakpad/src
)

target_link_libraries(external_breakpad
INTERFACE
    windows/common
    windows/handler/exception_handler
    windows/crash_generation/crash_generation_client
)

target_link_directories(external_breakpad
INTERFACE
    ${libs_loc}/breakpad/src/out/$<IF:$<CONFIG:Debug>,Debug,Release>/obj/client
)
