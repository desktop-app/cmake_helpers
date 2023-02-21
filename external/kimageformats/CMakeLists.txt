# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if ((DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY) OR APPLE) # should work on macOS, but cmake rules aren't implemented yet
    add_library(external_kimageformats INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_kimageformats ALIAS external_kimageformats)
    return()
endif()

add_library(external_kimageformats STATIC)
add_library(desktop-app::external_kimageformats ALIAS external_kimageformats)
init_target(external_kimageformats "(external)")

set(kimageformats_loc ${third_party_loc}/kimageformats)
set(kimageformats_src ${kimageformats_loc}/src/imageformats)

set(kimageformats_srcs)
qt_wrap_cpp(kimageformats_srcs
    ${kimageformats_src}/avif_p.h
    ${kimageformats_src}/heif_p.h
    ${kimageformats_src}/jxl_p.h
TARGET
    external_kimageformats
DEPENDS
    ${kimageformats_src}/avif.json
    ${kimageformats_src}/heif.json
    ${kimageformats_src}/jxl.json
)

nice_target_sources(external_kimageformats ${kimageformats_src}
PRIVATE
    avif.cpp
    heif.cpp
    jxl.cpp
)
target_sources(external_kimageformats PRIVATE ${kimageformats_srcs})

target_compile_definitions(external_kimageformats
PRIVATE
    QT_STATICPLUGIN
    KIMG_JXL_API_VERSION=70
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
        ${libs_loc}/libde265/libde265/$<IF:$<CONFIG:Debug>,Debug,Release>/de265.lib
        ${libs_loc}/dav1d/builddir-$<IF:$<CONFIG:Debug>,debug,release>/src/libdav1d.a
        ${libs_loc}/libjxl/lib/$<IF:$<CONFIG:Debug>,Debug,Release>/jxl-static.lib
        ${libs_loc}/libjxl/lib/$<IF:$<CONFIG:Debug>,Debug,Release>/jxl_threads-static.lib
        ${libs_loc}/libjxl/third_party/highway/$<IF:$<CONFIG:Debug>,Debug,Release>/hwy.lib
        ${libs_loc}/libjxl/third_party/brotli/$<IF:$<CONFIG:Debug>,Debug,Release>/brotlidec-static.lib
        ${libs_loc}/libjxl/third_party/brotli/$<IF:$<CONFIG:Debug>,Debug,Release>/brotlienc-static.lib
        ${libs_loc}/libjxl/third_party/brotli/$<IF:$<CONFIG:Debug>,Debug,Release>/brotlicommon-static.lib
    )
else()
    target_link_static_libraries(external_kimageformats
    PRIVATE
        avif
        heif
        jxl
        jxl_threads
        dav1d
        de265
        brotlidec-static
        brotlienc-static
        brotlicommon-static
        hwy
        lcms2
    )
endif()
