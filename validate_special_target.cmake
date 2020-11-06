# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

set(DESKTOP_APP_SPECIAL_TARGET "" CACHE STRING "Use special platform target, like 'macstore' for Mac App Store.")

function(report_bad_special_target)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
        message(FATAL_ERROR "Bad special target '${DESKTOP_APP_SPECIAL_TARGET}'")
    endif()
endfunction()

if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "osx")
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12 CACHE STRING "Minimum OS X deployment version" FORCE)
else()
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.10 CACHE STRING "Minimum OS X deployment version" FORCE)
endif()

if (WIN32)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp64"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "win"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "win64")
        report_bad_special_target()
    endif()
elseif (APPLE)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "osx"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "macstore"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "mac")
        report_bad_special_target()
    endif()
else()
    set(LINUX 1)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "linux"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "linux32")
        report_bad_special_target()
    endif()
endif()
