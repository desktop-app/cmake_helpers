# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

include(CMakeDependentOption)

set(DESKTOP_APP_SPECIAL_TARGET "" CACHE STRING "Use special platform target, like 'macstore' for Mac App Store.")

set(build_macstore 0)
set(build_winstore 0) # 32 or 64 bit
set(build_win64 0) # normal or uwp
set(build_winstore64 0)

if (WIN32)
    if (DESKTOP_APP_SPECIAL_TARGET STREQUAL "win64")
        set(build_win64 1)
    elseif (DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp")
        set(build_winstore 1)
    elseif (DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp64")
        set(build_win64 1)
        set(build_winstore 1)
        set(build_winstore64 1)
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(build_win64 1)
    endif()
elseif (APPLE)
    if (DESKTOP_APP_SPECIAL_TARGET STREQUAL "macstore")
        set(build_macstore 1)
    endif()
else()
    if (DESKTOP_APP_SPECIAL_TARGET)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_AR "gcc-ar")
            set(CMAKE_RANLIB "gcc-ranlib")
            set(CMAKE_NM "gcc-nm")
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(CMAKE_AR "llvm-ar")
            set(CMAKE_RANLIB "llvm-ranlib")
            set(CMAKE_NM "llvm-nm")
        endif()
    endif()
endif()

if (build_win64)
    get_filename_component(libs_loc "../Libraries/win64" REALPATH)
else()
    get_filename_component(libs_loc "../Libraries" REALPATH)
endif()

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

if (NOT DESKTOP_APP_USE_PACKAGED)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12 CACHE STRING "Minimum macOS deployment version" FORCE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Target macOS architectures" FORCE)
endif()

if (WIN32)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp64"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "win"
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "win64")
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
