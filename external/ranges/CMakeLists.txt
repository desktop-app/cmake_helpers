# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_ranges INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_ranges ALIAS external_ranges)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(range-v3 REQUIRED)
    target_link_libraries(external_ranges INTERFACE range-v3::range-v3)
else()
    target_include_directories(external_ranges SYSTEM
    INTERFACE
        ${libs_loc}/range-v3/include
    )
endif()

if (WIN32)
    target_compile_options(external_ranges
    INTERFACE
        /experimental:preprocessor # need for range-v3 see https://github.com/ericniebler/range-v3#supported-compilers
        /wd5105 # needed for `/experimental:preprocessor`, suppressing C5105 "macro expansion producing 'defined' has undefined behavior"
    )
endif()