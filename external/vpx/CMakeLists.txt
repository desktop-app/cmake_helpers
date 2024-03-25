# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_vpx INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_vpx ALIAS external_vpx)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBVPX IMPORTED_TARGET vpx>=1.10.0)

    target_link_libraries(external_vpx INTERFACE PkgConfig::LIBVPX)
    return()
endif()

add_library(external_vpx STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_vpx ALIAS external_vpx)

if (WIN32)
    target_include_directories(external_vpx SYSTEM
    INTERFACE
        ${libs_loc}/local/include
    )
    if (build_win64)
        set(libvpx_release_folder x64)
    else()
        set(libvpx_release_folder Win32)
    endif()
    set(vpx_lib_loc ${libs_loc}/local/lib/${libvpx_release_folder})
    set_target_properties(external_vpx PROPERTIES
        IMPORTED_LOCATION "${vpx_lib_loc}/vpxmt.lib"
        IMPORTED_LOCATION_DEBUG "${vpx_lib_loc}/vpxmt.lib"
    )
elseif (APPLE)
    target_include_directories(external_vpx SYSTEM
    INTERFACE
        ${libs_loc}/local/include
    )
    set_target_properties(external_vpx PROPERTIES
        IMPORTED_LOCATION ${libs_loc}/local/lib/libvpx.a
    )
else()
    find_library(DESKTOP_APP_VPX_LIBRARIES libvpx.a REQUIRED)
    set_target_properties(external_vpx PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_VPX_LIBRARIES}"
    )
endif()
