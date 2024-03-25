# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (NOT DESKTOP_APP_USE_PACKAGED)
    if (DEFINED ENV{QT})
        set(qt_version $ENV{QT} CACHE STRING "Qt version" FORCE)
    endif()

    if (NOT DEFINED qt_version)
        message(FATAL_ERROR "Qt version is unknown, set `QT' environment variable")
    endif()

    if (WIN32)
        set(qt_loc ${libs_loc}/Qt-${qt_version})
    elseif (APPLE)
        set(qt_loc ${libs_loc}/local/Qt-${qt_version})
    else()
        set(qt_loc /usr/local/Qt-${qt_version})
    endif()

    set(CMAKE_PREFIX_PATH ${qt_loc} ${libs_loc}/local)
endif()

if (NOT DEFINED QT_VERSION_MAJOR)
    if (NOT WIN32)
        find_package(QT NAMES Qt6 COMPONENTS Core)
    endif()
    if (NOT QT_FOUND)
        find_package(QT NAMES Qt5 COMPONENTS Core)
    endif()
    if (NOT QT_FOUND)
        message(FATAL_ERROR "Neither Qt6 nor Qt5 is found")
    endif()
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Network Svg REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Quick QuickWidgets QUIET)

set(qt_version_6_5_or_greater 0)
if (QT_VERSION_MAJOR GREATER_EQUAL 6)
    if (QT_VERSION_MAJOR GREATER 6 OR Qt6_VERSION_MINOR GREATER_EQUAL 5)
        set(qt_version_6_5_or_greater 1)
    endif()
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS OpenGL OpenGLWidgets REQUIRED)
endif()

# QtWaylandScanner cmake integration from Qt 6 is used
cmake_dependent_option(DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION "Disable all code for Wayland integration." OFF "LINUX; qt_version_6_5_or_greater" ON)

if (LINUX)
    find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS DBus WaylandClient WaylandCompositor QUIET)

    if (NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS WaylandClient REQUIRED)
    endif()
endif()

set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "(gen)")
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "(gen)")
