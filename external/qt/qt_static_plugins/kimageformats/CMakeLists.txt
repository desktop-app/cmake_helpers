# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt_static_plugins_kimageformats STATIC)
add_library(desktop-app::external_qt_static_plugins_kimageformats ALIAS external_qt_static_plugins_kimageformats)
init_target(external_qt_static_plugins_kimageformats "(external)")

set(kimageformats_loc ${third_party_loc}/kimageformats)
set(kimageformats_src ${kimageformats_loc}/src/imageformats)

set_target_properties(external_qt_static_plugins_kimageformats PROPERTIES AUTOMOC ON)

nice_target_sources(external_qt_static_plugins_kimageformats ${kimageformats_src}
PRIVATE
    avif.cpp
    heif.cpp
    jxl.cpp
    qoi.cpp
    scanlineconverter.cpp
)

target_compile_definitions(external_qt_static_plugins_kimageformats
PRIVATE
    QT_STATICPLUGIN
)

target_link_libraries(external_qt_static_plugins_kimageformats
PRIVATE
    desktop-app::external_qt
)

if (DESKTOP_APP_USE_PACKAGED OR LINUX)
    find_package(libavif QUIET)
    find_package(libheif QUIET)
    find_package(PkgConfig)
    if (PkgConfig_FOUND)
        if (NOT libavif_FOUND)
            pkg_check_modules(DESKTOP_APP_AVIF IMPORTED_TARGET libavif)
        endif()
        pkg_check_modules(DESKTOP_APP_JXL IMPORTED_TARGET libjxl libjxl_threads)
    endif()

    if (NOT libavif_FOUND AND NOT DESKTOP_APP_AVIF_FOUND)
        remove_target_sources(external_qt_static_plugins_kimageformats ${kimageformats_src} avif.cpp)
    endif()

    if (NOT libheif_FOUND)
        remove_target_sources(external_qt_static_plugins_kimageformats ${kimageformats_src} heif.cpp)
    endif()

    if (NOT DESKTOP_APP_JXL_FOUND)
        remove_target_sources(external_qt_static_plugins_kimageformats ${kimageformats_src} jxl.cpp)
    endif()

    target_link_libraries(external_qt_static_plugins_kimageformats
    PRIVATE
        $<TARGET_NAME_IF_EXISTS:avif>
        $<TARGET_NAME_IF_EXISTS:PkgConfig::DESKTOP_APP_AVIF>
        $<TARGET_NAME_IF_EXISTS:heif>
        $<TARGET_NAME_IF_EXISTS:PkgConfig::DESKTOP_APP_JXL>
    )
elseif (WIN32)
    target_compile_definitions(external_qt_static_plugins_kimageformats
    PRIVATE
        JXL_STATIC_DEFINE
        JXL_THREADS_STATIC_DEFINE
        JXL_CMS_STATIC_DEFINE
        LIBHEIF_STATIC_BUILD
    )

    target_include_directories(external_qt_static_plugins_kimageformats SYSTEM
    PRIVATE
        ${libs_loc}/local/include
    )

    target_link_libraries(external_qt_static_plugins_kimageformats
    PRIVATE
        ${libs_loc}/libavif/$<IF:$<CONFIG:Debug>,Debug,Release>/avif.lib
        ${libs_loc}/libheif/libheif/$<IF:$<CONFIG:Debug>,Debug,Release>/heif.lib
        ${libs_loc}/libde265/libde265/$<IF:$<CONFIG:Debug>,Debug,Release>/libde265.lib
        ${libs_loc}/dav1d/builddir-$<IF:$<CONFIG:Debug>,debug,release>/src/libdav1d.a
        ${libs_loc}/libjxl/lib/$<IF:$<CONFIG:Debug>,Debug,Release>/jxl.lib
        ${libs_loc}/libjxl/lib/$<IF:$<CONFIG:Debug>,Debug,Release>/jxl_cms.lib
        ${libs_loc}/libjxl/lib/$<IF:$<CONFIG:Debug>,Debug,Release>/jxl_threads.lib
        ${libs_loc}/libjxl/third_party/highway/$<IF:$<CONFIG:Debug>,Debug,Release>/hwy.lib
        ${libs_loc}/libjxl/third_party/brotli/$<IF:$<CONFIG:Debug>,Debug,Release>/brotlidec.lib
        ${libs_loc}/libjxl/third_party/brotli/$<IF:$<CONFIG:Debug>,Debug,Release>/brotlienc.lib
        ${libs_loc}/libjxl/third_party/brotli/$<IF:$<CONFIG:Debug>,Debug,Release>/brotlicommon.lib
    )
elseif (APPLE)
    target_compile_definitions(external_qt_static_plugins_kimageformats
    PRIVATE
        JXL_STATIC_DEFINE
        JXL_THREADS_STATIC_DEFINE
        JXL_CMS_STATIC_DEFINE
        LIBHEIF_STATIC_BUILD
    )

    target_include_directories(external_qt_static_plugins_kimageformats SYSTEM
    PRIVATE
        ${libs_loc}/local/include
    )
    target_link_libraries(external_qt_static_plugins_kimageformats
    PRIVATE
        ${libs_loc}/local/lib/libavif.a
        ${libs_loc}/local/lib/libheif.a
        ${libs_loc}/local/lib/libde265.a
        ${libs_loc}/local/lib/libdav1d.a
        ${libs_loc}/local/lib/libjxl.a
        ${libs_loc}/local/lib/libjxl_cms.a
        ${libs_loc}/local/lib/libjxl_threads.a
        ${libs_loc}/local/lib/libhwy.a
        ${libs_loc}/local/lib/libbrotlidec.a
        ${libs_loc}/local/lib/libbrotlienc.a
        ${libs_loc}/local/lib/libbrotlicommon.a
        ${libs_loc}/local/lib/liblcms2.a
    )
endif()

add_library(external_qt_static_plugins_kimageformats_init OBJECT)
add_library(desktop-app::external_qt_static_plugins_kimageformats_init ALIAS external_qt_static_plugins_kimageformats_init)
init_target(external_qt_static_plugins_kimageformats_init "(external)")

nice_target_sources(external_qt_static_plugins_kimageformats_init ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    init.cpp
)

target_link_libraries(external_qt_static_plugins_kimageformats_init
PRIVATE
    desktop-app::external_qt
)

if (DESKTOP_APP_USE_PACKAGED OR LINUX)
    if (NOT libavif_FOUND AND NOT DESKTOP_APP_AVIF_FOUND)
        target_compile_definitions(external_qt_static_plugins_kimageformats_init PRIVATE QT_STATIC_PLUGINS_DISABLE_AVIF)
    endif()

    if (NOT libheif_FOUND)
        target_compile_definitions(external_qt_static_plugins_kimageformats_init PRIVATE QT_STATIC_PLUGINS_DISABLE_HEIF)
    endif()

    if (NOT DESKTOP_APP_JXL_FOUND)
        target_compile_definitions(external_qt_static_plugins_kimageformats_init PRIVATE QT_STATIC_PLUGINS_DISABLE_JXL)
    endif()
endif()

target_link_libraries(external_qt_static_plugins_kimageformats
INTERFACE
    external_qt_static_plugins_kimageformats_init
    $<TARGET_OBJECTS:external_qt_static_plugins_kimageformats_init>
)
