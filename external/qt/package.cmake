# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (NOT DESKTOP_APP_USE_PACKAGED)
    if (DESKTOP_APP_QT6)
        set(qt_version 6.3.1)
    else()
        set(qt_version 5.15.4)
    endif()

    if (WIN32)
        set(qt_loc ${libs_loc}/Qt-${qt_version})
    elseif (APPLE)
        set(qt_loc ${libs_loc}/local/Qt-${qt_version})
    else()
        set(qt_loc /usr/local/desktop-app/Qt-${qt_version})
    endif()

    set(CMAKE_PREFIX_PATH ${qt_loc})
endif()

if (DESKTOP_APP_QT6)
    find_package(Qt6 COMPONENTS Core Core5Compat Gui OpenGL Widgets OpenGLWidgets Network Svg REQUIRED)
else()
    find_package(Qt5 COMPONENTS Core Gui Widgets Network Svg REQUIRED)
endif()

if (LINUX)
    if (NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
        find_package(ECM QUIET)
        if (ECM_FOUND)
            list(PREPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
        endif()

        if (DESKTOP_APP_QT6)
            find_package(Qt6 COMPONENTS WaylandClient REQUIRED)
            find_package(Qt6 OPTIONAL_COMPONENTS WaylandGlobalPrivate QUIET)
        else()
            find_package(Qt5 COMPONENTS WaylandClient REQUIRED)
            find_package(Qt5 OPTIONAL_COMPONENTS XkbCommonSupport QUIET)
            find_package(QtWaylandScanner REQUIRED)
        endif()

        find_package(WaylandProtocols QUIET)
        if (NOT WaylandProtocols_FOUND)
            set(WaylandProtocols_DATADIR ${third_party_loc}/wayland-protocols)
        endif()
        message(STATUS "Found WaylandProtocols: ${WaylandProtocols_DATADIR}")

        find_package(PlasmaWaylandProtocols QUIET)
        if (NOT PlasmaWaylandProtocols_FOUND)
            set(PLASMA_WAYLAND_PROTOCOLS_DIR ${third_party_loc}/plasma-wayland-protocols/src/protocols)
        endif()
        message(STATUS "Found PlasmaWaylandProtocols: ${PLASMA_WAYLAND_PROTOCOLS_DIR}")
    endif()

    if ((NOT DESKTOP_APP_USE_PACKAGED
                OR (DESKTOP_APP_USE_PACKAGED AND DESKTOP_APP_USE_PACKAGED_LAZY))
            AND NOT DESKTOP_APP_DISABLE_DBUS_INTEGRATION)
        if (DESKTOP_APP_QT6)
            find_package(Qt6 COMPONENTS DBus REQUIRED)
        else()
            find_package(Qt5 COMPONENTS DBus REQUIRED)
        endif()
    elseif (NOT DESKTOP_APP_USE_PACKAGED)
        if (DESKTOP_APP_QT6)
            find_package(Qt6 OPTIONAL_COMPONENTS DBus QUIET)
        else()
            find_package(Qt5 OPTIONAL_COMPONENTS DBus QUIET)
        endif()
    endif()
endif()

set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "(gen)")
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "(gen)")
