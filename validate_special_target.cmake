# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

include(CMakeDependentOption)

set(DESKTOP_APP_SPECIAL_TARGET "" CACHE STRING "Use special platform target, like 'macstore' for Mac App Store.")

get_filename_component(libs_loc "../Libraries" REALPATH)
set(libs_loc_exists 0)
if (EXISTS ${libs_loc})
    set(libs_loc_exists 1)
endif()
cmake_dependent_option(DESKTOP_APP_USE_PACKAGED "Find libraries using CMake instead of exact paths." OFF libs_loc_exists ON)

function(report_bad_special_target)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
        message(FATAL_ERROR "Bad special target '${DESKTOP_APP_SPECIAL_TARGET}'")
    endif()
endfunction()

if (APPLE AND NOT DESKTOP_APP_USE_PACKAGED)
    set(macos_deployment_target 10.13)
    if (NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
        set(macos_sdk macosx)
        if (CMAKE_OSX_SYSROOT)
            set(macos_sdk "${CMAKE_OSX_SYSROOT}")
        endif()
        execute_process(
            COMMAND xcrun --sdk "${macos_sdk}" --show-sdk-path
            OUTPUT_VARIABLE macos_sdk_path
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE macos_sdk_result
        )
        if (macos_sdk_result EQUAL 0)
            execute_process(
                COMMAND /usr/libexec/PlistBuddy
                    -c "Print :SupportedTargets:macosx:MinimumDeploymentTarget"
                    "${macos_sdk_path}/SDKSettings.plist"
                OUTPUT_VARIABLE macos_sdk_minimum
                OUTPUT_STRIP_TRAILING_WHITESPACE
                RESULT_VARIABLE macos_sdk_minimum_result
            )
            if (macos_sdk_minimum_result EQUAL 0
                AND macos_sdk_minimum VERSION_GREATER macos_deployment_target)
                set(macos_deployment_target "${macos_sdk_minimum}")
            endif()
        endif()
    endif()
    set(CMAKE_OSX_DEPLOYMENT_TARGET "${macos_deployment_target}" CACHE STRING "Minimum macOS deployment version")
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Target macOS architectures")
endif()

if (NOT DEFINED MSVC)
    set(MSVC 0)
endif()

if (WIN32)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp64"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "win"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "win64"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "winarm"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwparm")
        report_bad_special_target()
    endif()
elseif (APPLE)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "macstore"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "mac")
        report_bad_special_target()
    endif()
else()
    set(LINUX 1)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "linux")
        report_bad_special_target()
    endif()
endif()
