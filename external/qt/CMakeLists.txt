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
        $<TARGET_NAME_IF_EXISTS:Qt::Core5Compat>
        Qt::Gui
        Qt::GuiPrivate
        $<TARGET_NAME_IF_EXISTS:Qt::OpenGL>
        Qt::Widgets
        $<TARGET_NAME_IF_EXISTS:Qt::OpenGLWidgets>
        Qt::Network
        Qt::Svg
        $<TARGET_NAME_IF_EXISTS:Qt::DBus>
        $<TARGET_NAME_IF_EXISTS:Qt::XkbCommonSupport>
        $<TARGET_NAME_IF_EXISTS:Qt::XkbCommonSupportPrivate>
        $<TARGET_NAME_IF_EXISTS:Qt::WaylandClient>
        $<TARGET_NAME_IF_EXISTS:Qt::WaylandClientPrivate>
        $<TARGET_NAME_IF_EXISTS:Qt::WaylandGlobalPrivate>
    )

    return()
endif()

target_include_directories(external_qt SYSTEM
INTERFACE
    ${qt_loc}/include
    ${qt_loc}/include/QtCore
    $<$<TARGET_EXISTS:Qt::Core5Compat>:${qt_loc}/include/QtCore5Compat>
    ${qt_loc}/include/QtGui
    $<$<TARGET_EXISTS:Qt::OpenGL>:${qt_loc}/include/QtOpenGL>
    ${qt_loc}/include/QtWidgets
    $<$<TARGET_EXISTS:Qt::OpenGLWidgets>:${qt_loc}/include/QtOpenGLWidgets>
    ${qt_loc}/include/QtSvg
    $<$<TARGET_EXISTS:Qt::DBus>:${qt_loc}/include/QtDBus>
    $<$<TARGET_EXISTS:Qt::XkbCommonSupport>:${qt_loc}/include/QtXkbCommonSupport>
    $<$<TARGET_EXISTS:Qt::WaylandClient>:${qt_loc}/include/QtWaylandClient>
    ${qt_loc}/include/QtCore/${qt_version}
    ${qt_loc}/include/QtGui/${qt_version}
    $<$<TARGET_EXISTS:Qt::XkbCommonSupportPrivate>:${qt_loc}/include/QtXkbCommonSupport/${qt_version}>
    $<$<TARGET_EXISTS:Qt::WaylandClientPrivate>:${qt_loc}/include/QtWaylandClient/${qt_version}>
    $<$<TARGET_EXISTS:Qt::WaylandGlobalPrivate>:${qt_loc}/include/QtWaylandGlobal/${qt_version}>
    ${qt_loc}/include/QtCore/${qt_version}/QtCore
    ${qt_loc}/include/QtGui/${qt_version}/QtGui
    $<$<TARGET_EXISTS:Qt::XkbCommonSupportPrivate>:${qt_loc}/include/QtXkbCommonSupport/${qt_version}/QtXkbCommonSupport>
    $<$<TARGET_EXISTS:Qt::WaylandClientPrivate>:${qt_loc}/include/QtWaylandClient/${qt_version}/QtWaylandClient>
    $<$<TARGET_EXISTS:Qt::WaylandGlobalPrivate>:${qt_loc}/include/QtWaylandGlobal/${qt_version}/QtWaylandGlobal>
)

target_compile_definitions(external_qt
INTERFACE
    _REENTRANT
    QT_STATICPLUGIN
    QT_PLUGIN
    QT_CORE_LIB
    $<$<TARGET_EXISTS:Qt::Core5Compat>:QT_CORE5COMPAT_LIB>
    QT_GUI_LIB
    $<$<TARGET_EXISTS:Qt::OpenGL>:QT_OPENGL_LIB>
    QT_WIDGETS_LIB
    $<$<TARGET_EXISTS:Qt::OpenGLWidgets>:QT_OPENGLWIDGETS_LIB>
    QT_NETWORK_LIB
    QT_SVG_LIB
    $<$<TARGET_EXISTS:Qt::DBus>:QT_DBUS_LIB>
    $<$<TARGET_EXISTS:Qt::XkbCommonSupport>:QT_XKBCOMMON_SUPPORT_LIB>
    $<$<TARGET_EXISTS:Qt::WaylandClient>:QT_WAYLANDCLIENT_LIB>
    $<$<TARGET_EXISTS:Qt::WaylandGlobalPrivate>:QT_WAYLANDGLOBAL_LIB>
)

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
    lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Svg
    lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Network
    lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Widgets
    lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Gui
    lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Core
)

if (TARGET Qt::Core5Compat)
    list(PREPEND common_qt_libs
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Core5Compat
    )
endif()

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
    foreach (lib ${qt_libs})
        list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
    endforeach()
    list(APPEND qt_libs_list
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::QCocoaIntegrationPlugin_resources_1>
    )
else()
    set(qt_libs
        plugins/platforminputcontexts/${qt_lib_prefix}composeplatforminputcontextplugin
        plugins/platformthemes/${qt_lib_prefix}qgtk3
        plugins/platforms/${qt_lib_prefix}qxcb
        plugins/tls/${qt_lib_prefix}qopensslbackend
        plugins/xcbglintegrations/${qt_lib_prefix}qxcb-egl-integration
        plugins/xcbglintegrations/${qt_lib_prefix}qxcb-glx-integration
        plugins/iconengines/${qt_lib_prefix}qsvgicon
        lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}XcbQpa
        ${common_qt_libs}
    )
    if (TARGET Qt::DBus)
        list(APPEND qt_libs
            plugins/platforminputcontexts/${qt_lib_prefix}ibusplatforminputcontextplugin
            plugins/platformthemes/${qt_lib_prefix}qxdgdesktopportal
            lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}DBus
            lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}Core
        )
    endif()
    if (NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
        list(PREPEND qt_libs
            plugins/platforms/${qt_lib_prefix}qwayland-generic
            plugins/platforms/${qt_lib_prefix}qwayland-egl
            plugins/wayland-graphics-integration-client/${qt_lib_prefix}qt-plugin-wayland-egl
            plugins/wayland-shell-integration/${qt_lib_prefix}xdg-shell
            plugins/wayland-decoration-client/${qt_lib_prefix}bradient
            lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}WaylandEglClientHwIntegration
            lib/${qt_lib_prefix}Qt${QT_VERSION_MAJOR}WaylandClient
        )
    endif()
    foreach (lib ${qt_libs})
        list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
    endforeach()
endif()

if (QT_VERSION_MAJOR GREATER_EQUAL 6)
    list(APPEND qt_libs_list
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::Gui_resources_1>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::Widgets_resources_1>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::Widgets_resources_2>
        $<TARGET_OBJECTS:Qt${QT_VERSION_MAJOR}::Widgets_resources_3>
    )
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
        EGL
        GL
        xcb
        X11
        X11-xcb
        glib-2.0
    )
endif()
