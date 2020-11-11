# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

set(no_special_target 0)
if (DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
    set(no_special_target 1)
endif()

set(linux_special_target 0)
if (LINUX AND NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
    set(linux_special_target 1)
endif()

set(osx_special_target 0)
if (DESKTOP_APP_SPECIAL_TARGET STREQUAL "osx")
    set(osx_special_target 1)
endif()

set(disable_autoupdate 0)
if (DESKTOP_APP_SPECIAL_TARGET STREQUAL ""
    OR DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp"
    OR DESKTOP_APP_SPECIAL_TARGET STREQUAL "uwp64"
    OR DESKTOP_APP_SPECIAL_TARGET STREQUAL "macstore")
    set(disable_autoupdate 1)
endif()

option(DESKTOP_APP_LOTTIE_USE_CACHE "Use caching in lottie animations." ON)
option(DESKTOP_APP_DISABLE_DBUS_INTEGRATION "Disable all code for D-Bus integration (Linux only)." OFF)
option(DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION "Disable all code for Wayland integration (Linux only)." OFF)
option(DESKTOP_APP_DISABLE_WEBRTC_INTEGRATION "Disable all code for WebRTC integration." OFF)
option(DESKTOP_APP_USE_GLIBC_WRAPS "Use wraps for new GLIBC features." OFF)
option(DESKTOP_APP_USE_PACKAGED "Find libraries using CMake instead of exact paths." ${no_special_target})
option(DESKTOP_APP_USE_PACKAGED_LAZY "Bundle recommended Qt plugins for self-contained packages. (Linux only)" OFF)
option(DESKTOP_APP_USE_PACKAGED_LAZY_PLATFORMTHEMES "Bundle recommended Qt platform themes for self-contained packages. (Linux only)" ${DESKTOP_APP_USE_PACKAGED_LAZY})
option(DESKTOP_APP_USE_PACKAGED_FFMPEG_STATIC "Link ffmpeg statically in packaged mode." OFF)
option(DESKTOP_APP_DISABLE_SPELLCHECK "Disable spellcheck library." ${osx_special_target})
option(DESKTOP_APP_DISABLE_CRASH_REPORTS "Disable crash report generation." ${no_special_target})
option(DESKTOP_APP_DISABLE_AUTOUPDATE "Disable autoupdate." ${disable_autoupdate})
option(DESKTOP_APP_USE_HUNSPELL_ONLY "Disable system spellchecker and use bundled Hunspell only. (For debugging purposes)" OFF)
option(DESKTOP_APP_USE_ENCHANT "Use Enchant instead of bundled Hunspell. (Linux only)" OFF)
set(DESKTOP_APP_QTWAYLANDCLIENT_PRIVATE_HEADERS "" CACHE STRING "QtWaylandClient headers location.")

set(dont_bundle_fonts 0)
if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    set(dont_bundle_fonts 1)
endif()
option(DESKTOP_APP_USE_PACKAGED_FONTS "Use preinstalled fonts instead of bundled one." ${dont_bundle_fonts})

set(add_hunspell_library 0)
if ((WIN32
  OR (LINUX AND NOT DESKTOP_APP_USE_ENCHANT)
  OR DESKTOP_APP_USE_HUNSPELL_ONLY)
  AND NOT DESKTOP_APP_DISABLE_SPELLCHECK)
    set(add_hunspell_library 1)
endif()

set(build_osx 0)
set(build_macstore 0)
set(build_winstore 0) # 32 or 64 bit
set(build_linux32 0)
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
    endif()
elseif (APPLE)
    if (DESKTOP_APP_SPECIAL_TARGET STREQUAL "osx")
        set(build_osx 1)
    elseif (DESKTOP_APP_SPECIAL_TARGET STREQUAL "macstore")
        set(build_macstore 1)
    endif()
else()
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
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
    endif()
    if (DESKTOP_APP_SPECIAL_TARGET)
        set(CMAKE_AR "gcc-ar")
        set(CMAKE_RANLIB "gcc-ranlib")
        set(CMAKE_NM "gcc-nm")
    endif()
endif()

if (build_win64)
    get_filename_component(libs_loc "../Libraries/win64" REALPATH)
elseif (NOT APPLE OR build_osx)
    get_filename_component(libs_loc "../Libraries" REALPATH)
else()
    get_filename_component(libs_loc "../Libraries/macos" REALPATH)
endif()
