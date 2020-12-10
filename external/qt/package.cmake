# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (NOT DESKTOP_APP_USE_PACKAGED)
    if (NOT APPLE OR NOT build_osx)
        set(qt_version 6.0.0)
    else()
        set(qt_version 5.6.2)
    endif()

    if (WIN32)
        set(qt_loc ${libs_loc}/Qt-${qt_version})
    else()
        set(qt_loc /usr/local/desktop-app/Qt-${qt_version})
    endif()

    set(OPENSSL_FOUND ON)
    set(OPENSSL_INCLUDE_DIR ${libs_loc}/openssl_1_1_1/include)
    set(OPENSSL_CRYPTO_LIBRARY ${libs_loc}/openssl_1_1_1/out32.dbg/libcrypto.lib)
    set(LIB_EAY_LIBRARY_DEBUG ${libs_loc}/openssl_1_1_1/out32.dbg/libcrypto.lib)
    set(LIB_EAY_LIBRARY_RELEASE ${libs_loc}/openssl_1_1_1/out32/libcrypto.lib)
    set(SSL_EAY_LIBRARY_DEBUG ${libs_loc}/openssl_1_1_1/out32.dbg/libssl.lib)
    set(SSL_EAY_LIBRARY_RELEASE ${libs_loc}/openssl_1_1_1/out32/libssl.lib)
    set(JPEG_INCLUDE_DIR ${libs_loc}/mozjpeg)
    set(JPEG_LIBRARY_DEBUG ${libs_loc}/mozjpeg/Debug/jpeg-static.lib)
    set(JPEG_LIBRARY_RELEASE ${libs_loc}/mozjpeg/Release/jpeg-static.lib)

    set(Qt6_DIR ${qt_loc}/lib/cmake/Qt6)
    set(Qt6Core_DIR ${qt_loc}/lib/cmake/Qt6Core)
    set(Qt6CoreTools_DIR ${qt_loc}/lib/cmake/Qt6CoreTools)
    set(Qt6Core5Compat_DIR ${qt_loc}/lib/cmake/Qt6Core5Compat)
    set(Qt6Gui_DIR ${qt_loc}/lib/cmake/Qt6Gui)
    set(Qt6GuiTools_DIR ${qt_loc}/lib/cmake/Qt6GuiTools)
    set(Qt6Widgets_DIR ${qt_loc}/lib/cmake/Qt6Widgets)
    set(Qt6WidgetsTools_DIR ${qt_loc}/lib/cmake/Qt6WidgetsTools)
    set(Qt6Network_DIR ${qt_loc}/lib/cmake/Qt6Network)
    set(Qt6BundledPcre2_DIR ${qt_loc}/lib/cmake/Qt6BundledPcre2)
    set(Qt6BundledLibpng_DIR ${qt_loc}/lib/cmake/Qt6BundledLibpng)
    set(Qt6BundledHarfbuzz_DIR ${qt_loc}/lib/cmake/Qt6BundledHarfbuzz)
    set(Qt6BundledFreetype_DIR ${qt_loc}/lib/cmake/Qt6BundledFreetype)
endif()

find_package(Qt6 COMPONENTS Core Core5Compat Gui Widgets Network REQUIRED)
find_package(Qt6Gui COMPONENTS QWebpPlugin REQUIRED)

if (LINUX)
    if (NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
        find_package(Qt6 COMPONENTS WaylandClient REQUIRED)
        find_package(Qt6 OPTIONAL_COMPONENTS XkbCommonSupport QUIET)
    endif()

    if (NOT DESKTOP_APP_USE_PACKAGED OR DESKTOP_APP_USE_PACKAGED_LAZY_PLATFORMTHEMES)
        find_package(Qt6 COMPONENTS Svg REQUIRED)
    endif()

    if (DESKTOP_APP_DISABLE_DBUS_INTEGRATION)
        find_package(Qt6 OPTIONAL_COMPONENTS DBus QUIET)
    else()
        find_package(Qt6 COMPONENTS DBus REQUIRED)
    endif()
endif()

set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "(gen)")
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "(gen)")
