# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_kimageformats INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_kimageformats ALIAS external_kimageformats)
    return()
endif()

add_library(external_kimageformats STATIC)
add_library(desktop-app::external_kimageformats ALIAS external_kimageformats)
init_target(external_kimageformats "(external)")

set(kimageformats_loc ${third_party_loc}/kimageformats)
set(kimageformats_src ${kimageformats_loc}/src/imageformats)

set_target_properties(external_kimageformats PROPERTIES AUTOMOC ON)

nice_target_sources(external_kimageformats ${kimageformats_src}
PRIVATE
    avif.cpp
    heif.cpp
    jxl.cpp
    qoi.cpp
    scanlineconverter.cpp
)

target_compile_definitions(external_kimageformats
PRIVATE
    QT_STATICPLUGIN
)

target_link_libraries(external_kimageformats
PRIVATE
    desktop-app::external_qt
)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(libavif REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBHEIF REQUIRED IMPORTED_TARGET libheif)
    pkg_check_modules(LIBJXL REQUIRED IMPORTED_TARGET libjxl)
    pkg_check_modules(LIBJXL_THREADS REQUIRED IMPORTED_TARGET libjxl_threads)

    target_link_libraries(external_kimageformats
    PRIVATE
        avif
        PkgConfig::LIBHEIF
        PkgConfig::LIBJXL
        PkgConfig::LIBJXL_THREADS
    )
elseif (WIN32)
    target_compile_definitions(external_kimageformats
    PRIVATE
        JXL_STATIC_DEFINE
        JXL_THREADS_STATIC_DEFINE
        LIBHEIF_STATIC_BUILD
    )

    target_include_directories(external_kimageformats SYSTEM
    PRIVATE
        ${libs_loc}/local/include
    )

    target_link_libraries(external_kimageformats
    PRIVATE
        ${libs_loc}/libavif/$<IF:$<CONFIG:Debug>,Debug,Release>/avif.lib
        ${libs_loc}/libheif/libheif/$<IF:$<CONFIG:Debug>,Debug,Release>/heif.lib
        ${libs_loc}/libde265/libde265/$<IF:$<CONFIG:Debug>,Debug,Release>/libde265.lib
        ${libs_loc}/dav1d/builddir-$<IF:$<CONFIG:Debug>,debug,release>/src/libdav1d.a
        ${libs_loc}/libjxl/lib/$<IF:$<CONFIG:Debug>,Debug,Release>/jxl-static.lib
        ${libs_loc}/libjxl/lib/$<IF:$<CONFIG:Debug>,Debug,Release>/jxl_threads-static.lib
        ${libs_loc}/libjxl/third_party/highway/$<IF:$<CONFIG:Debug>,Debug,Release>/hwy.lib
        ${libs_loc}/libjxl/third_party/brotli/$<IF:$<CONFIG:Debug>,Debug,Release>/brotlidec-static.lib
        ${libs_loc}/libjxl/third_party/brotli/$<IF:$<CONFIG:Debug>,Debug,Release>/brotlienc-static.lib
        ${libs_loc}/libjxl/third_party/brotli/$<IF:$<CONFIG:Debug>,Debug,Release>/brotlicommon-static.lib
    )
elseif (APPLE)
    target_compile_definitions(external_kimageformats
    PRIVATE
        JXL_STATIC_DEFINE
        JXL_THREADS_STATIC_DEFINE
        LIBHEIF_STATIC_BUILD
    )

    target_include_directories(external_kimageformats SYSTEM
    PRIVATE
        ${libs_loc}/local/include
    )
    target_link_libraries(external_kimageformats
    PRIVATE
        ${libs_loc}/local/lib/libavif.a
        ${libs_loc}/local/lib/libheif.a
        ${libs_loc}/local/lib/libde265.a
        ${libs_loc}/local/lib/libdav1d.a
        ${libs_loc}/local/lib/libjxl.a
        ${libs_loc}/local/lib/libjxl_threads.a
        ${libs_loc}/local/lib/libhwy.a
        ${libs_loc}/local/lib/libbrotlidec-static.a
        ${libs_loc}/local/lib/libbrotlienc-static.a
        ${libs_loc}/local/lib/libbrotlicommon-static.a
    )
else()
    target_link_static_libraries(external_kimageformats
    PRIVATE
        avif
        heif
        jxl
        jxl_cms
        jxl_threads
        dav1d
        de265
        brotlidec
        brotlienc
        brotlicommon
        hwy
        lcms2
    )
endif()

add_library(external_kimageformats_init OBJECT)
add_library(desktop-app::external_kimageformats_init ALIAS external_kimageformats_init)
init_target(external_kimageformats_init "(external)")

nice_target_sources(external_kimageformats_init ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    init.cpp
)

target_link_libraries(external_kimageformats_init
PRIVATE
    desktop-app::external_qt
)

target_link_libraries(external_kimageformats
INTERFACE
    external_kimageformats_init
    $<TARGET_OBJECTS:external_kimageformats_init>
)
