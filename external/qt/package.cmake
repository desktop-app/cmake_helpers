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
        set(qt_loc /usr/local/desktop-app/Qt-${qt_version})
    endif()

    set(CMAKE_PREFIX_PATH ${qt_loc})
endif()

if (NOT DEFINED QT_VERSION_MAJOR)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Network Svg REQUIRED)

set(qt_version_6_or_greater 0)
if (QT_VERSION_MAJOR GREATER_EQUAL 6)
    set(qt_version_6_or_greater 1)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core5Compat OpenGL OpenGLWidgets REQUIRED)
endif()

cmake_dependent_option(DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION "Disable all code for Wayland integration." OFF LINUX ON)

if (LINUX)
    if (NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS WaylandClient REQUIRED)

        find_package(ECM QUIET)
        if (ECM_FOUND)
            list(PREPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
        endif()
        if (QT_VERSION_MAJOR GREATER_EQUAL 5)
            find_package(QtWaylandScanner REQUIRED)

            # Imitate QtWayland's generator function by means of extra CMake modules.
            function(qt_generate_wayland_protocol_client_sources target)
                cmake_parse_arguments(GEN "" "" FILES ${ARGN})
                foreach(xmldef ${GEN_FILES})
                    get_filename_component(base ${xmldef} NAME_WE)
                    ecm_add_qtwayland_client_protocol(generated PROTOCOL ${xmldef} BASENAME ${base})
                endforeach()
                target_sources(${target} PRIVATE ${generated})
                target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
            endfunction()
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
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS DBus REQUIRED)
    elseif (NOT DESKTOP_APP_USE_PACKAGED)
        find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS DBus QUIET)
    endif()
endif()

set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "(gen)")
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "(gen)")
