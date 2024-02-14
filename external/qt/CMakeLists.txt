# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_qt ALIAS external_qt)

add_subdirectory(qt_static_plugins)

if (DESKTOP_APP_USE_PACKAGED)
    target_link_libraries(external_qt
    INTERFACE
        Qt::Core
        Qt::CorePrivate
        Qt::Gui
        Qt::GuiPrivate
        $<TARGET_NAME_IF_EXISTS:Qt::OpenGL>
        Qt::Widgets
        Qt::WidgetsPrivate
        $<TARGET_NAME_IF_EXISTS:Qt::OpenGLWidgets>
        Qt::Network
        Qt::Svg
        $<TARGET_NAME_IF_EXISTS:Qt::DBus>
        $<TARGET_NAME_IF_EXISTS:Qt::Quick>
        $<TARGET_NAME_IF_EXISTS:Qt::QuickWidgets>
        $<TARGET_NAME_IF_EXISTS:Qt::WaylandCompositor>
    )

    return()
endif()

target_include_directories(external_qt SYSTEM
INTERFACE
    ${qt_loc}/include
    ${qt_loc}/include/QtCore
    ${qt_loc}/include/QtGui
    $<$<TARGET_EXISTS:Qt::OpenGL>:${qt_loc}/include/QtOpenGL>
    ${qt_loc}/include/QtWidgets
    $<$<TARGET_EXISTS:Qt::OpenGLWidgets>:${qt_loc}/include/QtOpenGLWidgets>
    ${qt_loc}/include/QtSvg
    $<$<TARGET_EXISTS:Qt::DBus>:${qt_loc}/include/QtDBus>
    $<$<TARGET_EXISTS:Qt::Quick>:${qt_loc}/include/QtQuick>
    $<$<TARGET_EXISTS:Qt::QuickWidgets>:${qt_loc}/include/QtQuickWidgets>
    $<$<TARGET_EXISTS:Qt::WaylandCompositor>:${qt_loc}/include/QtWaylandCompositor>
    ${qt_loc}/include/QtCore/${qt_version}
    ${qt_loc}/include/QtGui/${qt_version}
    ${qt_loc}/include/QtWidgets/${qt_version}
    ${qt_loc}/include/QtCore/${qt_version}/QtCore
    ${qt_loc}/include/QtGui/${qt_version}/QtGui
    ${qt_loc}/include/QtWidgets/${qt_version}/QtWidgets
)

target_compile_definitions(external_qt
INTERFACE
    _REENTRANT
    QT_STATICPLUGIN
    QT_PLUGIN
    QT_CORE_LIB
    QT_GUI_LIB
    $<$<TARGET_EXISTS:Qt::OpenGL>:QT_OPENGL_LIB>
    QT_WIDGETS_LIB
    $<$<TARGET_EXISTS:Qt::OpenGLWidgets>:QT_OPENGLWIDGETS_LIB>
    QT_NETWORK_LIB
    QT_SVG_LIB
    $<$<TARGET_EXISTS:Qt::DBus>:QT_DBUS_LIB>
    $<$<TARGET_EXISTS:Qt::Quick>:QT_QUICK_LIB>
    $<$<TARGET_EXISTS:Qt::QuickWidgets>:QT_QUICKWIDGETS_LIB>
    $<$<TARGET_EXISTS:Qt::WaylandCompositor>:QT_WAYLANDCOMPOSITOR_LIB>
)

if (WIN32)
    set(qt_lib_prefix "")
    set(qt_lib_suffix $<$<CONFIG:Debug>:d>.lib)

    target_compile_definitions(external_qt
    INTERFACE
        _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    )
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
    lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Svg
    lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Network
    lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Widgets
    lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Gui
    lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Core
)

if (TARGET Qt::OpenGL)
    list(PREPEND common_qt_libs
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}OpenGL
    )
endif()

if (TARGET Qt::OpenGLWidgets)
    list(PREPEND common_qt_libs
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}OpenGLWidgets
    )
endif()

if (TARGET Qt::Quick)
    list(PREPEND common_qt_libs
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Quick
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Qml
    )
endif()

if (TARGET Qt::QuickWidgets)
    list(PREPEND common_qt_libs
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}QuickWidgets
    )
endif()

if (QT_VERSION_MAJOR GREATER_EQUAL 6)
    list(APPEND common_qt_libs
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}BundledHarfbuzz
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}BundledLibpng
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}BundledPcre2
    )
else()
    list(APPEND common_qt_libs
        lib/${qt_lib_prefix}qtharfbuzz
        lib/${qt_lib_prefix}qtlibpng
        lib/${qt_lib_prefix}qtpcre2
    )

endif()

set(qt_libs_list "")
if (QT_VERSION_MAJOR GREATER_EQUAL 6)
    list(APPEND qt_libs_list
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QWebpPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QJpegPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QGifPlugin_init>
    )
endif()

if (WIN32)
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/mkspecs/win32-msvc
    )
    set(qt_libs
        plugins/bearer/${qt_lib_prefix}qgenericbearer
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}AccessibilitySupport
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}FontDatabaseSupport
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}EventDispatcherSupport
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}ThemeSupport
        ${common_qt_libs}
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}WindowsUIAutomationSupport
        lib/${qt_lib_prefix}qtmain
        lib/${qt_lib_prefix}qtfreetype
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
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}BundledFreetype
        plugins/platforms/${qt_lib_prefix}qcocoa
        plugins/tls/${qt_lib_prefix}qsecuretransportbackend
        plugins/networkinformation/${qt_lib_prefix}qscnetworkreachability
    )
    list(APPEND qt_libs_list
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QCocoaIntegrationPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QSecureTransportBackendPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QSCNetworkReachabilityNetworkInformationPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QCocoaIntegrationPlugin_resources_1>
    )
    foreach (lib ${qt_libs})
        list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
    endforeach()
else()
    set(qt_libs
        plugins/platforms/${qt_lib_prefix}qxcb
        plugins/xcbglintegrations/${qt_lib_prefix}qxcb-egl-integration
        plugins/xcbglintegrations/${qt_lib_prefix}qxcb-glx-integration
        plugins/platforminputcontexts/${qt_lib_prefix}composeplatforminputcontextplugin
        plugins/iconengines/${qt_lib_prefix}qsvgicon
        plugins/platformthemes/${qt_lib_prefix}qgtk3
        plugins/tls/${qt_lib_prefix}qopensslbackend
        plugins/networkinformation/${qt_lib_prefix}qglib
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}XcbQpa
        ${common_qt_libs}
    )
    list(APPEND qt_libs_list
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QXcbIntegrationPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QXcbEglIntegrationPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QXcbGlxIntegrationPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QComposePlatformInputContextPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QSvgIconPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QGtk3ThemePlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QTlsBackendOpenSSLPlugin_init>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QGlibNetworkInformationPlugin_init>
    )
    if (TARGET Qt::DBus)
        list(PREPEND qt_libs
            plugins/platforminputcontexts/${qt_lib_prefix}ibusplatforminputcontextplugin
            plugins/platformthemes/${qt_lib_prefix}qxdgdesktopportal
        )
        list(APPEND qt_libs
            lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}DBus
            lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Core
        )
        list(APPEND qt_libs_list
            $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QIbusPlatformInputContextPlugin_init>
            $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QXdgDesktopPortalThemePlugin_init>
        )
    endif()
    if (TARGET Qt::WaylandClient)
        list(PREPEND qt_libs
            plugins/platforms/${qt_lib_prefix}qwayland-generic
            plugins/platforms/${qt_lib_prefix}qwayland-egl
            plugins/wayland-graphics-integration-client/${qt_lib_prefix}qt-plugin-wayland-egl
            plugins/wayland-shell-integration/${qt_lib_prefix}xdg-shell
            plugins/wayland-decoration-client/${qt_lib_prefix}bradient
            lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}WaylandEglClientHwIntegration
            lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}WaylandClient
        )
        list(APPEND qt_libs_list
            $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QWaylandIntegrationPlugin_init>
            $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QWaylandEglPlatformIntegrationPlugin_init>
            $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QWaylandEglClientBufferPlugin_init>
            $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QWaylandXdgShellIntegrationPlugin_init>
            $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QWaylandBradientDecorationPlugin_init>
        )
    endif()
    if (TARGET Qt::WaylandCompositor)
        list(PREPEND qt_libs
            plugins/wayland-graphics-integration-server/${qt_lib_prefix}qt-wayland-compositor-wayland-egl
            lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}WaylandEglCompositorHwIntegration
            lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}WaylandCompositor
        )
        list(APPEND qt_libs_list
            $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QWaylandEglClientBufferIntegrationPlugin_init>
            $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::WaylandCompositor_resources_1>
        )
    endif()
    foreach (lib ${qt_libs})
        list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
    endforeach()
endif()

if (QT_VERSION_MAJOR GREATER_EQUAL 6)
    list(APPEND qt_libs_list
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::Gui_resources_1>
        $<$<TARGET_EXISTS:Qt${QT_VERSION_MAJOR}::Gui_resources_2>:$<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::Gui_resources_2>>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::Widgets_resources_1>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::Widgets_resources_2>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::Widgets_resources_3>
    )

    if (TARGET Qt::Quick)
        list(APPEND qt_libs_list
            $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::Quick_resources_2>
        )
    endif()
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

if (WIN32)
    if (build_win64)
        set(libwebp_release_folder x64)
    else()
        set(libwebp_release_folder x86)
    endif()

    set(webp_lib_loc ${libs_loc}/libwebp/out/$<IF:$<CONFIG:Debug>,debug,release>-static/${libwebp_release_folder}/lib)

    target_link_libraries(external_qt
    INTERFACE
        ${webp_lib_loc}/libwebp$<$<CONFIG:Debug>:_debug>.lib
        ${webp_lib_loc}/libwebpdemux$<$<CONFIG:Debug>:_debug>.lib
        ${webp_lib_loc}/libwebpmux$<$<CONFIG:Debug>:_debug>.lib
    )
elseif (APPLE)
    target_link_libraries(external_qt
    INTERFACE
        ${libs_loc}/local/lib/libwebp.a
        ${libs_loc}/local/lib/libwebpdemux.a
        ${libs_loc}/local/lib/libwebpmux.a
        ${libs_loc}/local/lib/libsharpyuv.a
    )
 elseif (LINUX)
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/mkspecs/linux-g++
    )
    target_link_static_libraries(external_qt
    INTERFACE
        lcms2
        xkbcommon
        xkbcommon-x11
        xcb-cursor
        xcb-glx
        xcb-xkb
        xcb-randr
        xcb-icccm
        xcb-shm
        xcb-render
        xcb-image
        xcb-xfixes
        xcb-shape
        xcb-sync
        xcb-util
        xcb-render-util
        xcb-keysyms
        webpmux
        webpdemux
        webp
        sharpyuv
    )
    if (TARGET Qt::WaylandClient)
        target_link_optional_libraries(external_qt
        INTERFACE
            wayland-egl
            wayland-cursor
            wayland-client
        )
    endif()
    if (TARGET Qt::WaylandCompositor)
        target_link_optional_libraries(external_qt
        INTERFACE
            wayland-server
        )
    endif()
    target_link_optional_libraries(external_qt
    INTERFACE
        gtk-3
        gdk-3
        gdk_pixbuf-2.0
        pango-1.0
        EGL
        GL
        X11-xcb
        xcb
    )
    target_link_libraries(external_qt
    INTERFACE
        fontconfig
        freetype
        X11
        gio-2.0
        gobject-2.0
        glib-2.0
        rt
    )
endif()
