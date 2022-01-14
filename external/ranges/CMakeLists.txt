# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_ranges INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_ranges ALIAS external_ranges)

if (DESKTOP_APP_USE_PACKAGED)
    if (DESKTOP_APP_USE_PACKAGED_LAZY)
        find_package(range-v3 QUIET)
    else()
        find_package(range-v3)
    endif()

    if (range-v3_FOUND)
        target_link_libraries(external_ranges INTERFACE range-v3::range-v3)
        return()
    endif()
endif()

target_include_directories(external_ranges SYSTEM
INTERFACE
    ${third_party_loc}/range-v3/include
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(external_ranges
    INTERFACE
        /Zc:preprocessor # need for range-v3 see https://github.com/ericniebler/range-v3#supported-compilers
        /wd5105 # needed for `/Zc:preprocessor`, suppressing C5105 "macro expansion producing 'defined' has undefined behavior"
    )
endif()