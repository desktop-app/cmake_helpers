# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_tmc INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_tmc ALIAS external_tmc)

target_compile_definitions(external_tmc
INTERFACE
    TMC_WORK_ITEM=FUNCORO
)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(TooManyCooks QUIET)
    if (TooManyCooks_FOUND)
        target_link_libraries(external_tmc INTERFACE TooManyCooks::TooManyCooks)
        return()
    endif()
endif()

target_include_directories(external_tmc SYSTEM
INTERFACE
    ${third_party_loc}/TooManyCooks/include
)