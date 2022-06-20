# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

set(no_special_target 0)
if (DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
    set(no_special_target 1)
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
option(DESKTOP_APP_DISABLE_X11_INTEGRATION "Disable all code for X11 integration (Linux only)." OFF)
option(DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION "Disable all code for Wayland integration (Linux only)." OFF)
option(DESKTOP_APP_USE_ALLOCATION_TRACER "Use simple allocation tracer (Linux only)." OFF)
option(DESKTOP_APP_USE_PACKAGED_LAZY "Bundle recommended Qt plugins for self-contained packages. (Linux only)" OFF)
option(DESKTOP_APP_USE_PACKAGED_FONTS "Use preinstalled fonts instead of bundled patched ones." OFF)
option(DESKTOP_APP_USE_PACKAGED_RLOTTIE "Find rlottie using CMake instead of bundled patched one." OFF)
option(DESKTOP_APP_DISABLE_SPELLCHECK "Disable spellcheck library." OFF)
option(DESKTOP_APP_DISABLE_CRASH_REPORTS "Disable crash report generation." ${no_special_target})
option(DESKTOP_APP_DISABLE_AUTOUPDATE "Disable autoupdate." ${disable_autoupdate})
option(DESKTOP_APP_USE_HUNSPELL_ONLY "Disable system spellchecker and use bundled Hunspell only. (For debugging purposes)" OFF)
option(DESKTOP_APP_USE_ENCHANT "Use Enchant instead of bundled Hunspell. (Linux only)" OFF)
option(DESKTOP_APP_NO_PDB "Disable PDB file generation. (Windows only)" OFF)
option(DESKTOP_APP_DISABLE_JEMALLOC "Disable jemalloc, use system malloc (Linux only)." OFF)

if (DESKTOP_APP_USE_PACKAGED AND DEFINED CMAKE_OSX_ARCHITECTURES)
    set(DESKTOP_APP_MAC_ARCH "${CMAKE_OSX_ARCHITECTURES}" CACHE STRING "Target macOS arch. (macOS only)")
else()
    set(DESKTOP_APP_MAC_ARCH "x86_64;arm64" CACHE STRING "Target macOS arch. (macOS only)")
endif()

# QtWaylandScanner cmake integration from Qt 6 is used
if (NOT DESKTOP_APP_QT6)
    set(DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION ON)
endif()

set(add_hunspell_library 0)
if ((WIN32
  OR (LINUX AND NOT DESKTOP_APP_USE_ENCHANT)
  OR DESKTOP_APP_USE_HUNSPELL_ONLY)
  AND NOT DESKTOP_APP_DISABLE_SPELLCHECK)
    set(add_hunspell_library 1)
endif()

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
