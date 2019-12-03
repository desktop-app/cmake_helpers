# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

set(DESKTOP_APP_SPECIAL_TARGET "" CACHE STRING "Use special platform target, like 'mas' for Mac App Store.")
option(DESKTOP_APP_DISABLE_CRASH_REPORTS "Disable crash report generation." OFF)
option(DESKTOP_APP_DISABLE_SPELLCHECK "Disable spellcheck library." OFF)
option(DESKTOP_APP_LOTTIE_USE_CACHE "Use caching in lottie animations." ON)

function(report_bad_special_target)
    message(FATAL_ERROR "Bad special target '${DESKTOP_APP_SPECIAL_TARGET}'")
endfunction()

if (DESKTOP_APP_SPECIAL_TARGET STREQUAL ""
    OR DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp"
    OR DESKTOP_APP_SPECIAL_TARGET STREQUAL "mas")
    set(disable_autoupdate 1)
endif()

if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "osx")
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12 CACHE STRING "Minimum OS X deployment version" FORCE)
else()
    set(DESKTOP_APP_DISABLE_SPELLCHECK ON)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.10 CACHE STRING "Minimum OS X deployment version" FORCE)
endif()

set(build_osx 0)
set(build_macstore 0)
set(build_winstore 0)
set(build_linux32 0)

if (WIN32)
    if (DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp")
        set(build_winstore 1)
    elseif (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL ""
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "win")
        report_bad_special_target()
    endif()
elseif (APPLE)
    if (DESKTOP_APP_SPECIAL_TARGET STREQUAL "osx")
        set(build_osx 1)
        set(CMAKE_OSX_DEPLOYMENT_TARGET 10.10)
    elseif (DESKTOP_APP_SPECIAL_TARGET STREQUAL "mas")
        set(build_macstore 1)
    elseif (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL ""
        AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "mac")
        report_bad_special_target()
    endif()
else()
    set(LINUX 1)
    execute_process(COMMAND uname -m OUTPUT_VARIABLE machine_uname)
    if (NOT ${machine_uname} MATCHES "x86_64" AND NOT ${machine_uname} MATCHES "aarch64")
        set(build_linux32 1)
    endif()
    if (DESKTOP_APP_SPECIAL_TARGET STREQUAL "linux")
        if (build_linux32)
            report_bad_special_target()
        endif()
    elseif (DESKTOP_APP_SPECIAL_TARGET STREQUAL "linux32")
        if (NOT build_linux32)
            report_bad_special_target()
        endif()
    elseif (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
        report_bad_special_target()
    endif()
    set(CMAKE_AR /usr/bin/gcc-ar)
    set(CMAKE_RANLIB /usr/bin/gcc-ranlib)
    set(CMAKE_NM /usr/bin/gcc-nm)
endif()

if (NOT APPLE OR build_osx)
    get_filename_component(libs_loc "../Libraries" REALPATH)
else()
    get_filename_component(libs_loc "../Libraries/macos" REALPATH)
endif()
get_filename_component(third_party_loc "Telegram/ThirdParty" REALPATH)
get_filename_component(submodules_loc "Telegram" REALPATH)
