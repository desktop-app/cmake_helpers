# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_qt ALIAS external_qt)

if (DESKTOP_APP_USE_PACKAGED)
    target_link_libraries(external_qt
    INTERFACE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
    )

    target_include_directories(external_qt
    INTERFACE
        ${Qt6Core_PRIVATE_INCLUDE_DIRS}
        ${Qt6Gui_PRIVATE_INCLUDE_DIRS}
    )

    if (Qt6DBus_FOUND)
        target_link_libraries(external_qt INTERFACE Qt6::DBus)
    endif()

    if (Qt6Svg_FOUND)
        target_link_libraries(external_qt INTERFACE Qt6::Svg)
    endif()

    if (LINUX AND DESKTOP_APP_USE_PACKAGED_LAZY_PLATFORMTHEMES)
        find_library(Qt6ThemeSupport_LIBRARY
        NAMES
            Qt6ThemeSupport
        REQUIRED
        )

        find_path(Qt6ThemeSupport_INCLUDE_DIR
        NAMES
            QtThemeSupport/private/qgenericunixthemes_p.h
        HINTS
            ${Qt6Gui_INCLUDE_DIRS}
        PATH_SUFFIXES
            QtThemeSupport/${Qt6Gui_VERSION}
        REQUIRED
        )

        target_link_libraries(external_qt
        INTERFACE
            ${Qt6ThemeSupport_LIBRARY}
        )

        target_include_directories(external_qt
        INTERFACE
            ${Qt6ThemeSupport_INCLUDE_DIR}
            ${Qt6ThemeSupport_INCLUDE_DIR}/QtThemeSupport
        )
    endif()

    if (Qt6XkbCommonSupport_FOUND)
        target_link_libraries(external_qt INTERFACE Qt6::XkbCommonSupport)

        target_include_directories(external_qt
        INTERFACE
            ${Qt6XkbCommonSupport_PRIVATE_INCLUDE_DIRS}
        )
    endif()

    if (Qt6WaylandClient_FOUND)
        target_link_libraries(external_qt INTERFACE Qt6::WaylandClient)

        if (DESKTOP_APP_QTWAYLANDCLIENT_PRIVATE_HEADERS)
            target_include_directories(external_qt
            INTERFACE
                ${DESKTOP_APP_QTWAYLANDCLIENT_PRIVATE_HEADERS}
                ${DESKTOP_APP_QTWAYLANDCLIENT_PRIVATE_HEADERS}/QtWaylandClient
            )
        else()
            target_include_directories(external_qt
            INTERFACE
                ${Qt6WaylandClient_PRIVATE_INCLUDE_DIRS}
            )
        endif()
    endif()
else()
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/include
        ${qt_loc}/include/QtCore
        ${qt_loc}/include/QtCore5Compat
        ${qt_loc}/include/QtGui
        ${qt_loc}/include/QtWidgets
        ${qt_loc}/include/QtCore/${qt_version}
        ${qt_loc}/include/QtGui/${qt_version}
        ${qt_loc}/include/QtCore/${qt_version}/QtCore
        ${qt_loc}/include/QtGui/${qt_version}/QtGui
    )

    if (Qt6DBus_FOUND)
        target_include_directories(external_qt SYSTEM
        INTERFACE
            ${qt_loc}/include/QtDBus
        )
    endif()

    if (Qt6XkbCommonSupport_FOUND)
        target_include_directories(external_qt SYSTEM
        INTERFACE
            ${qt_loc}/include/QtXkbCommonSupport
            ${qt_loc}/include/QtXkbCommonSupport/${qt_version}
            ${qt_loc}/include/QtXkbCommonSupport/${qt_version}/QtXkbCommonSupport
        )
    endif()

    if (Qt6WaylandClient_FOUND)
        target_include_directories(external_qt SYSTEM
        INTERFACE
            ${qt_loc}/include/QtWaylandClient
            ${qt_loc}/include/QtWaylandClient/${qt_version}
            ${qt_loc}/include/QtWaylandClient/${qt_version}/QtWaylandClient
        )
    endif()

    if (LINUX)
        target_include_directories(external_qt SYSTEM
        INTERFACE
            ${qt_loc}/include/QtSvg
            ${qt_loc}/include/QtThemeSupport
            ${qt_loc}/include/QtThemeSupport/${qt_version}
            ${qt_loc}/include/QtThemeSupport/${qt_version}/QtThemeSupport
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
    )

    if (Qt6DBus_FOUND)
        target_compile_definitions(external_qt
        INTERFACE
            QT_DBUS_LIB
        )
    endif()

    if (Qt6XkbCommonSupport_FOUND)
        target_compile_definitions(external_qt
        INTERFACE
            QT_XKBCOMMON_SUPPORT_LIB
        )
    endif()

    if (Qt6WaylandClient_FOUND)
        target_compile_definitions(external_qt
        INTERFACE
            QT_WAYLANDCLIENT_LIB
        )
    endif()

    if (LINUX)
        target_compile_definitions(external_qt
        INTERFACE
            QT_THEME_SUPPORT_LIB
            QT_SVG_LIB
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
        plugins/imageformats/${qt_lib_prefix}qwebp
        plugins/imageformats/${qt_lib_prefix}qgif
        plugins/imageformats/${qt_lib_prefix}qjpeg
        lib/${qt_lib_prefix}Qt6PrintSupport
        lib/${qt_lib_prefix}Qt6Network
        lib/${qt_lib_prefix}Qt6Widgets
        lib/${qt_lib_prefix}Qt6Gui
        lib/${qt_lib_prefix}Qt6BundledHarfbuzz
        lib/${qt_lib_prefix}Qt6BundledLibpng
        lib/${qt_lib_prefix}Qt6Core_qobject
    )

    set(qt_libs_list "")
    if (WIN32)
        set(qt_libs
            ${common_qt_libs}
            lib/${qt_lib_prefix}Qt6Core
            lib/${qt_lib_prefix}Qt6Core5Compat
            lib/${qt_lib_prefix}Qt6EntryPoint
            lib/${qt_lib_prefix}Qt6BundledFreetype
            lib/${qt_lib_prefix}Qt6BundledPcre2
            plugins/platforms/${qt_lib_prefix}qwindows
            plugins/platforms/${qt_lib_prefix}qdirect2d
        )
        foreach (lib ${qt_libs})
            list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
        endforeach()
        target_link_libraries(external_qt
        INTERFACE
            desktop-app::external_jpeg
            desktop-app::external_zlib
            ${libs_loc}/WebP/lib/webp.lib
            ${libs_loc}/WebP/lib/webpdemux.lib
            ${libs_loc}/WebP/lib/webpmux.lib
        )
    elseif (APPLE)
        if (NOT build_osx)
            set(qt_libs
                ${common_qt_libs}
                lib/${qt_lib_prefix}Qt6Core
                lib/${qt_lib_prefix}Qt6GraphicsSupport
                lib/${qt_lib_prefix}Qt6ClipboardSupport
                lib/${qt_lib_prefix}qtfreetype
                lib/${qt_lib_prefix}qtpcre2
                plugins/platforms/${qt_lib_prefix}qcocoa
                plugins/bearer/${qt_lib_prefix}qgenericbearer
            )
        else()
            set(qt_libs
                lib/${qt_lib_prefix}Qt6PrintSupport
                lib/${qt_lib_prefix}Qt6PlatformSupport
                lib/${qt_lib_prefix}Qt6Network
                lib/${qt_lib_prefix}Qt6Widgets
                lib/${qt_lib_prefix}Qt6Gui
                lib/${qt_lib_prefix}Qt6Core
                lib/${qt_lib_prefix}qtharfbuzzng
                lib/${qt_lib_prefix}qtfreetype
                lib/${qt_lib_prefix}qtpcre
                plugins/platforms/${qt_lib_prefix}qcocoa
                plugins/imageformats/${qt_lib_prefix}qwebp
                plugins/bearer/${qt_lib_prefix}qgenericbearer
            )
        endif()
        foreach (lib ${qt_libs})
            list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
        endforeach()
        target_link_libraries(external_qt
        INTERFACE
            desktop-app::external_jpeg
            desktop-app::external_zlib
            cups
        )
    else()
        set(qt_libs_dbus)
        set(qt_libs_dbus_support)
        set(qt_libs_dbus_plugins)
        set(qt_libs_dbus_bearers)
        if (Qt6DBus_FOUND)
            set(qt_libs_dbus lib/${qt_lib_prefix}Qt6DBus)
            set(qt_libs_dbus_support lib/${qt_lib_prefix}Qt6LinuxAccessibilitySupport)
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
        if (Qt6WaylandClient_FOUND)
            set(qt_libs_waylandclient lib/${qt_lib_prefix}Qt6WaylandClient)
            set(qt_libs_waylandclient_plugins
                plugins/wayland-decoration-client/${qt_lib_prefix}bradient
                plugins/wayland-shell-integration/${qt_lib_prefix}ivi-shell
                plugins/wayland-shell-integration/${qt_lib_prefix}wl-shell
                plugins/wayland-shell-integration/${qt_lib_prefix}xdg-shell-v5
                plugins/wayland-shell-integration/${qt_lib_prefix}xdg-shell-v6
                plugins/wayland-shell-integration/${qt_lib_prefix}xdg-shell
                plugins/wayland-graphics-integration-client/${qt_lib_prefix}qt-plugin-wayland-egl
                plugins/wayland-graphics-integration-client/${qt_lib_prefix}drm-egl-server
                plugins/wayland-graphics-integration-client/${qt_lib_prefix}dmabuf-server
                plugins/wayland-graphics-integration-client/${qt_lib_prefix}shm-emulation-server
                plugins/platforms/${qt_lib_prefix}qwayland-egl
                plugins/platforms/${qt_lib_prefix}qwayland-generic
            )
        endif()
        set(qt_libs
            plugins/platforminputcontexts/${qt_lib_prefix}composeplatforminputcontextplugin
            ${qt_libs_dbus_plugins}
            ${qt_libs_waylandclient_plugins}
            plugins/platforms/${qt_lib_prefix}qxcb
            plugins/imageformats/${qt_lib_prefix}qsvg
            plugins/iconengines/${qt_lib_prefix}qsvgicon
            lib/${qt_lib_prefix}Qt6XcbQpa
            lib/${qt_lib_prefix}Qt6EglSupport
            lib/${qt_lib_prefix}Qt6ServiceSupport
            lib/${qt_lib_prefix}Qt6EdidSupport
            lib/${qt_lib_prefix}Qt6XkbCommonSupport
            ${qt_libs_dbus_support}
            plugins/bearer/${qt_lib_prefix}qgenericbearer
            ${qt_libs_dbus_bearers}
            ${qt_libs_waylandclient}
            lib/${qt_lib_prefix}Qt6ServiceSupport
            ${common_qt_libs}
            ${qt_libs_dbus}
            lib/${qt_lib_prefix}Qt6Svg
            lib/${qt_lib_prefix}Qt6Core
            lib/${qt_lib_prefix}qtpcre2
        )
        foreach (lib ${qt_libs})
            list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
        endforeach()
    endif()

    target_link_libraries(external_qt
    INTERFACE
        ${qt_libs_list}
        $<TARGET_FILE:desktop-app::external_openssl_ssl>
        $<TARGET_FILE:desktop-app::external_openssl_crypto>
    )

    if (LINUX)
        target_include_directories(external_qt SYSTEM
        INTERFACE
            ${qt_loc}/mkspecs/linux-g++
        )
        target_link_options(external_qt
        INTERFACE
            -static-libstdc++
            -pthread
            -rdynamic
        )
        if (DESKTOP_APP_USE_GLIBC_WRAPS)
            target_link_options(external_qt
            INTERFACE
                -Wl,-wrap,aligned_alloc
                -Wl,-wrap,secure_getenv
                -Wl,-wrap,clock_gettime
                -Wl,--no-as-needed,-lrt
            )
            if (NOT build_linux32)
                target_link_options(external_qt
                INTERFACE
                    -Wl,-wrap,__divmodti4
                )
            else()
                target_link_options(external_qt
                INTERFACE
                    -Wl,-wrap,__divmoddi4
                )
            endif()
        endif()
        target_link_static_libraries(external_qt
        INTERFACE
            wayland-egl
            wayland-cursor
            wayland-client
            xkbcommon
            xkbcommon-x11
            Xext
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
            xcb-record
            ffi
        )
        if (DESKTOP_APP_USE_GLIBC_WRAPS)
            target_link_libraries(external_qt
            INTERFACE
                desktop-app::linux_glibc_wraps
                $<TARGET_FILE:desktop-app::linux_glibc_wraps>
            )
        endif()
        target_link_libraries(external_qt
        INTERFACE
            desktop-app::external_jpeg
            desktop-app::external_zlib
            ICE
            SM
            fontconfig
            freetype
            EGL
            GL
            xcb
            X11
            X11-xcb
            dl
            glib-2.0
            pthread
        )
    endif()
endif()
