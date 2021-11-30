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
        Qt6::Core5Compat
        Qt6::Gui
        Qt6::OpenGL
        Qt6::Widgets
        Qt6::OpenGLWidgets
        Qt6::Network
        Qt6::Svg
    )

    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${Qt6Core_PRIVATE_INCLUDE_DIRS}
        ${Qt6Gui_PRIVATE_INCLUDE_DIRS}
    )

    if (Qt6DBus_FOUND)
        target_link_libraries(external_qt INTERFACE Qt6::DBus)
    endif()

    if (LINUX AND NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
        target_link_libraries(external_qt INTERFACE Qt6::WaylandClient)

        target_include_directories(external_qt SYSTEM
        INTERFACE
            ${Qt6WaylandClient_PRIVATE_INCLUDE_DIRS}
        )
    endif()
else()
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/include
        ${qt_loc}/include/QtCore
        ${qt_loc}/include/QtCore5Compat
        ${qt_loc}/include/QtGui
        ${qt_loc}/include/QtOpenGL
        ${qt_loc}/include/QtWidgets
        ${qt_loc}/include/QtOpenGLWidgets
        ${qt_loc}/include/QtSvg
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
        QT_SVG_LIB
        QT_NETWORK_LIB
        QT_OPENGLWIDGETS_LIB
        QT_WIDGETS_LIB
        QT_OPENGL_LIB
        QT_GUI_LIB
        QT_CORE5COMPAT_LIB
        QT_CORE_LIB
    )

    if (Qt6DBus_FOUND)
        target_compile_definitions(external_qt
        INTERFACE
            QT_DBUS_LIB
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

    if (LINUX)
        set(qt_lib_objects objects-RelWithDebInfo)
    else()
        set(qt_lib_objects objects-$<IF:$<CONFIG:Debug>,Debug,RelWithDebInfo>)
    endif()

    set(common_qt_libs
        plugins/imageformats/${qt_lib_prefix}qwebp
        plugins/imageformats/${qt_lib_prefix}qgif
        plugins/imageformats/${qt_lib_prefix}qjpeg
        lib/${qt_lib_prefix}Qt6Svg
        lib/${qt_lib_prefix}Qt6Network
        lib/${qt_lib_prefix}Qt6OpenGLWidgets
        lib/${qt_lib_prefix}Qt6Widgets
        lib/${qt_lib_prefix}Qt6OpenGL
        lib/${qt_lib_prefix}Qt6Gui
        lib/${qt_lib_prefix}Qt6Core5Compat
        lib/${qt_lib_prefix}Qt6Core
        lib/${qt_lib_prefix}Qt6BundledHarfbuzz
        lib/${qt_lib_prefix}Qt6BundledLibpng
        lib/${qt_lib_prefix}Qt6BundledPcre2
    )

    set(qt_libs_list "")
    if (WIN32)
        set(qt_libs
            ${common_qt_libs}
            lib/${qt_lib_prefix}Qt6WindowsUIAutomationSupport
            lib/${qt_lib_prefix}qtmain
            lib/${qt_lib_prefix}Qt6BundledFreetype
            plugins/platforms/${qt_lib_prefix}qwindows
            plugins/tls/${qt_lib_prefix}qopensslbackend
        )
        foreach (lib ${qt_libs})
            list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
        endforeach()

        target_link_libraries(external_qt
        INTERFACE
            desktop-app::external_angle
        )
    elseif (APPLE)
        set(qt_libs
            ${common_qt_libs}
            lib/${qt_lib_prefix}Qt6BundledFreetype
            plugins/platforms/${qt_lib_prefix}qcocoa
            plugins/tls/${qt_lib_prefix}qsecuretransportbackend
            plugins/networkinformation/${qt_lib_prefix}qscnetworkreachability
        )
        foreach (lib ${qt_libs})
            list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
        endforeach()
        list(APPEND qt_libs_list
            "${qt_loc}/lib/${qt_lib_objects}/QCocoaIntegrationPlugin_resources_1/.rcc/qrc_qcocoaresources.cpp.o"
        )
    else()
        set(qt_libs_dbus)
        set(qt_libs_dbus_plugins)
        if (Qt6DBus_FOUND)
            set(qt_libs_dbus
                lib/${qt_lib_prefix}Qt6DBus
                lib/${qt_lib_prefix}Qt6Core
            )
            set(qt_libs_dbus_plugins
                plugins/platforminputcontexts/${qt_lib_prefix}ibusplatforminputcontextplugin
            )
        endif()
        set(qt_libs_waylandclient)
        set(qt_libs_waylandclient_plugins)
        if (NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
            set(qt_libs_waylandclient
                lib/${qt_lib_prefix}Qt6WaylandEglClientHwIntegration
                lib/${qt_lib_prefix}Qt6WaylandClient
            )
            set(qt_libs_waylandclient_plugins
                plugins/platforms/${qt_lib_prefix}qwayland-generic
                plugins/platforms/${qt_lib_prefix}qwayland-egl
                plugins/wayland-graphics-integration-client/${qt_lib_prefix}qt-plugin-wayland-egl
                plugins/wayland-decoration-client/${qt_lib_prefix}bradient
            )
        endif()
        set(qt_libs
            plugins/platforminputcontexts/${qt_lib_prefix}composeplatforminputcontextplugin
            ${qt_libs_dbus_plugins}
            ${qt_libs_waylandclient_plugins}
            plugins/platformthemes/${qt_lib_prefix}qgtk3
            plugins/platforms/${qt_lib_prefix}qxcb
            plugins/tls/${qt_lib_prefix}qopensslbackend
            plugins/xcbglintegrations/${qt_lib_prefix}qxcb-egl-integration
            plugins/xcbglintegrations/${qt_lib_prefix}qxcb-glx-integration
            plugins/iconengines/${qt_lib_prefix}qsvgicon
            lib/${qt_lib_prefix}Qt6XcbQpa
            ${qt_libs_waylandclient}
            ${common_qt_libs}
            ${qt_libs_dbus}
        )
        foreach (lib ${qt_libs})
            list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
        endforeach()
    endif()

    list(APPEND qt_libs_list
        "${qt_loc}/lib/${qt_lib_objects}/Gui_resources_1/.rcc/qrc_qpdf.cpp.o"
        "${qt_loc}/lib/${qt_lib_objects}/Widgets_resources_1/.rcc/qrc_qstyle.cpp.o"
        "${qt_loc}/lib/${qt_lib_objects}/Widgets_resources_2/.rcc/qrc_qstyle1.cpp.o"
        "${qt_loc}/lib/${qt_lib_objects}/Widgets_resources_3/.rcc/qrc_qmessagebox.cpp.o"
    )

    target_link_libraries(external_qt
    INTERFACE
        ${qt_libs_list}
        $<TARGET_FILE:desktop-app::external_openssl_ssl>
        $<TARGET_FILE:desktop-app::external_openssl_crypto>
        desktop-app::external_jpeg
        desktop-app::external_zlib
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
            xcb-xinput
            xcb-shape
            xcb-sync
            xcb-util
            xcb-render-util
            xcb-keysyms
            z
        )
        if (NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
            target_link_libraries(external_qt
            INTERFACE
                desktop-app::linux_wayland_helper
                $<TARGET_FILE:desktop-app::linux_wayland_helper>
            )
        endif()
        target_link_libraries(external_qt
        INTERFACE
            desktop-app::linux_xcb_helper
            $<TARGET_FILE:desktop-app::linux_xcb_helper>
            fontconfig
            freetype
            xcb
            X11
            X11-xcb
            ${CMAKE_DL_LIBS}
            pthread
        )
    endif()
endif()
