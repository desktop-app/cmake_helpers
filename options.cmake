# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(common_options INTERFACE)

target_compile_features(common_options
INTERFACE
    cxx_std_17
)

target_compile_definitions(common_options
INTERFACE
    UNICODE
)

if (WIN32)
    include(cmake/options_win.cmake)
else()
endif()
