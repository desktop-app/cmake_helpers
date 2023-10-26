# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_boost_regex INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_boost_regex ALIAS external_boost_regex)

target_compile_definitions(external_boost_regex
INTERFACE
    BOOST_NO_INTRINSIC_WCHAR_T
    BOOST_REGEX_NO_W32
)

if (DESKTOP_APP_USE_PACKAGED OR LINUX)
    find_package(Boost COMPONENTS regex REQUIRED)
    target_link_libraries(external_boost_regex INTERFACE Boost::regex)
    return()
endif()

target_include_directories(external_boost_regex SYSTEM
INTERFACE
    ${libs_loc}/regex/include
)
