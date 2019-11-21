# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (build_osx)
    target_compile_definitions(common_options INTERFACE OS_OSX)
elseif (build_macstore)
    target_compile_definitions(common_options INTERFACE OS_MAC_STORE)
endif()
