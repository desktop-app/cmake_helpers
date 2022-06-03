# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_subdirectory(qt_static_plugins)
if (DESKTOP_APP_QT6)
    add_subdirectory(qt6)
    return()
endif()

add_library(external_qt INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_qt ALIAS external_qt)

if (DESKTOP_APP_USE_PACKAGED)
    target_link_libraries(external_qt
    INTERFACE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::Svg
    )

    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${Qt5Core_PRIVATE_INCLUDE_DIRS}
        ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    )

    if (Qt5DBus_FOUND)
        target_link_libraries(external_qt INTERFACE Qt5::DBus)
    endif()

    if (Qt5XkbCommonSupport_FOUND)
        target_link_libraries(external_qt INTERFACE Qt5::XkbCommonSupport)

        target_include_directories(external_qt SYSTEM
        INTERFACE
            ${Qt5XkbCommonSupport_PRIVATE_INCLUDE_DIRS}
        )
    endif()

    if (LINUX AND NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
        target_link_libraries(external_qt INTERFACE Qt5::WaylandClient)

        target_include_directories(external_qt SYSTEM
        INTERFACE
            ${Qt5WaylandClient_PRIVATE_INCLUDE_DIRS}
        )
    endif()

    return()
endif()

target_include_directories(external_qt SYSTEM
INTERFACE
    ${qt_loc}/include
    ${qt_loc}/include/QtCore
    ${qt_loc}/include/QtGui
    ${qt_loc}/include/QtWidgets
    ${qt_loc}/include/QtSvg
    ${qt_loc}/include/QtCore/${qt_version}
    ${qt_loc}/include/QtGui/${qt_version}
    ${qt_loc}/include/QtCore/${qt_version}/QtCore
    ${qt_loc}/include/QtGui/${qt_version}/QtGui
)

if (Qt5DBus_FOUND)
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/include/QtDBus
    )
endif()

if (Qt5XkbCommonSupport_FOUND)
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/include/QtXkbCommonSupport
        ${qt_loc}/include/QtXkbCommonSupport/${qt_version}
        ${qt_loc}/include/QtXkbCommonSupport/${qt_version}/QtXkbCommonSupport
    )
endif()

if (LINUX AND NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/include/QtWaylandClient
        ${qt_loc}/include/QtWaylandClient/${qt_version}
        ${qt_loc}/include/QtWaylandClient/${qt_version}/QtWaylandClient
    )
endif()

target_compile_definitions(external_qt
INTERFACE
    _REENTRANT
    QT_STATICPLUGIN
    QT_PLUGIN
    QT_WIDGETS_LIB
    QT_NETWORK_LIB
    QT_GUI_LIB
    QT_CORE_LIB
    QT_SVG_LIB
)

if (Qt5DBus_FOUND)
    target_compile_definitions(external_qt
    INTERFACE
        QT_DBUS_LIB
    )
endif()

if (Qt5XkbCommonSupport_FOUND)
    target_compile_definitions(external_qt
    INTERFACE
        QT_XKBCOMMON_SUPPORT_LIB
    )
endif()

if (LINUX AND NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
    target_compile_definitions(external_qt
    INTERFACE
        QT_WAYLANDCLIENT_LIB
    )
endif()

if (WIN32)
    set(qt_lib_prefix "")
    set(qt_lib_suffix $<$<CONFIG:Debug>:d>.lib)
else()
    set(qt_lib_prefix lib)
    if (APPLE)
        set(qt_lib_suffix $<$<CONFIG:Debug>:_debug>.a)
    else()
        set(qt_lib_suffix .a)
    endif()
endif()

set(common_qt_libs
    plugins/bearer/${qt_lib_prefix}qgenericbearer
    plugins/imageformats/${qt_lib_prefix}qwebp
    plugins/imageformats/${qt_lib_prefix}qgif
    plugins/imageformats/${qt_lib_prefix}qjpeg
    lib/${qt_lib_prefix}Qt5PrintSupport
    lib/${qt_lib_prefix}Qt5AccessibilitySupport
    lib/${qt_lib_prefix}Qt5FontDatabaseSupport
    lib/${qt_lib_prefix}Qt5EventDispatcherSupport
    lib/${qt_lib_prefix}Qt5ThemeSupport
    lib/${qt_lib_prefix}Qt5Network
    lib/${qt_lib_prefix}Qt5Widgets
    lib/${qt_lib_prefix}Qt5Gui
    lib/${qt_lib_prefix}Qt5Svg
    lib/${qt_lib_prefix}qtharfbuzz
    lib/${qt_lib_prefix}qtlibpng
)

set(qt_libs_list "")
if (WIN32)
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/mkspecs/win32-msvc
    )
    set(qt_libs
        ${common_qt_libs}
        lib/${qt_lib_prefix}Qt5Core
        lib/${qt_lib_prefix}Qt5WindowsUIAutomationSupport
        lib/${qt_lib_prefix}qtmain
        lib/${qt_lib_prefix}qtfreetype
        lib/${qt_lib_prefix}qtpcre2
        plugins/platforms/${qt_lib_prefix}qwindows
    )
    foreach (lib ${qt_libs})
        list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
    endforeach()

    target_link_libraries(external_qt
    INTERFACE
        desktop-app::external_angle
    )
elseif (APPLE)
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/mkspecs/macx-clang
    )
    set(qt_libs
        ${common_qt_libs}
        lib/${qt_lib_prefix}Qt5Core
        lib/${qt_lib_prefix}Qt5GraphicsSupport
        lib/${qt_lib_prefix}Qt5ClipboardSupport
        lib/${qt_lib_prefix}qtfreetype
        lib/${qt_lib_prefix}qtpcre2
        plugins/platforms/${qt_lib_prefix}qcocoa
    )
    foreach (lib ${qt_libs})
        list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
    endforeach()
    target_link_libraries(external_qt
    INTERFACE
        cups
    )
else()
    set(qt_libs_dbus)
    set(qt_libs_dbus_support)
    set(qt_libs_dbus_plugins)
    set(qt_libs_dbus_bearers)
    if (Qt5DBus_FOUND)
        set(qt_libs_dbus lib/${qt_lib_prefix}Qt5DBus)
        set(qt_libs_dbus_support lib/${qt_lib_prefix}Qt5LinuxAccessibilitySupport)
        set(qt_libs_dbus_plugins
            plugins/platforminputcontexts/${qt_lib_prefix}ibusplatforminputcontextplugin
            plugins/platformthemes/${qt_lib_prefix}qxdgdesktopportal
        )
        set(qt_libs_dbus_bearers
            plugins/bearer/${qt_lib_prefix}qconnmanbearer
            plugins/bearer/${qt_lib_prefix}qnmbearer
        )
    endif()
    set(qt_libs_waylandclient)
    set(qt_libs_waylandclient_plugins)
    if (NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
        set(qt_libs_waylandclient lib/${qt_lib_prefix}Qt5WaylandClient)
        set(qt_libs_waylandclient_plugins
            plugins/platforms/${qt_lib_prefix}qwayland-generic
            plugins/platforms/${qt_lib_prefix}qwayland-egl
            plugins/wayland-graphics-integration-client/${qt_lib_prefix}qt-plugin-wayland-egl
            plugins/wayland-shell-integration/${qt_lib_prefix}xdg-shell
            plugins/wayland-decoration-client/${qt_lib_prefix}bradient
        )
    endif()
    set(qt_libs
        plugins/platforminputcontexts/${qt_lib_prefix}composeplatforminputcontextplugin
        ${qt_libs_dbus_plugins}
        ${qt_libs_waylandclient_plugins}
        plugins/platformthemes/${qt_lib_prefix}qgtk3
        plugins/platforms/${qt_lib_prefix}qxcb
        plugins/xcbglintegrations/${qt_lib_prefix}qxcb-egl-integration
        plugins/xcbglintegrations/${qt_lib_prefix}qxcb-glx-integration
        plugins/iconengines/${qt_lib_prefix}qsvgicon
        lib/${qt_lib_prefix}Qt5XcbQpa
        lib/${qt_lib_prefix}Qt5EglSupport
        lib/${qt_lib_prefix}Qt5GlxSupport
        lib/${qt_lib_prefix}Qt5ServiceSupport
        lib/${qt_lib_prefix}Qt5EdidSupport
        lib/${qt_lib_prefix}Qt5XkbCommonSupport
        ${qt_libs_dbus_support}
        ${qt_libs_dbus_bearers}
        ${qt_libs_waylandclient}
        lib/${qt_lib_prefix}Qt5ServiceSupport
        ${common_qt_libs}
        ${qt_libs_dbus}
        lib/${qt_lib_prefix}Qt5Core
        lib/${qt_lib_prefix}qtpcre2
    )
    foreach (lib ${qt_libs})
        list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
    endforeach()
endif()

target_link_libraries(external_qt
INTERFACE
    ${qt_libs_list}
    $<LINK_ONLY:desktop-app::external_openssl>
    $<TARGET_FILE:desktop-app::external_openssl_ssl>
    $<TARGET_FILE:desktop-app::external_openssl_crypto>
    $<LINK_ONLY:desktop-app::external_jpeg>
    $<TARGET_FILE:desktop-app::external_jpeg>
    $<LINK_ONLY:desktop-app::external_zlib>
    $<TARGET_FILE:desktop-app::external_zlib>
)

if (LINUX)
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/mkspecs/linux-g++
    )
    target_link_static_libraries(external_qt
    INTERFACE
        proxy
        epoxy
        xkbcommon
        xkbcommon-x11
        xcb-glx
        xcb-xkb
        xcb-randr
        xcb-icccm
        xcb-shm
        xcb-render
        xcb-image
        xcb-xfixes
        xcb-xinerama
        xcb-xinput
        xcb-shape
        xcb-sync
        xcb-util
        xcb-render-util
        xcb-keysyms
    )
    if (NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
        target_link_libraries(external_qt
        INTERFACE
            desktop-app::linux_wayland_helper
        )
    endif()
    target_link_libraries(external_qt
    INTERFACE
        desktop-app::linux_xcb_helper
        fontconfig
        freetype
        xcb
        X11
        X11-xcb
    )
endif()
