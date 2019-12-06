# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_expected INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_expected ALIAS external_expected)

target_include_directories(external_expected SYSTEM
INTERFACE
    ${third_party_loc}/expected/include
)
