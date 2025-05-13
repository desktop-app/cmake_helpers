# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_openh264 INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_openh264 ALIAS external_openh264)

    find_package(PkgConfig)
    if (PkgConfig_FOUND)
        pkg_check_modules(DESKTOP_APP_OPENH264 IMPORTED_TARGET openh264)
    endif()

    target_link_libraries(external_openh264 INTERFACE PkgConfig::DESKTOP_APP_OPENH264)
    return()
endif()

add_library(external_openh264 STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_openh264 ALIAS external_openh264)

if (WIN32)
    target_include_directories(external_openh264 SYSTEM
    INTERFACE
        ${libs_loc}/local/include
    )
    set_target_properties(external_openh264 PROPERTIES
        IMPORTED_LOCATION "${libs_loc}/openh264/builddir-release/libopenh264.a"
        IMPORTED_LOCATION_DEBUG "${libs_loc}/openh264/builddir-debug/libopenh264.a"
    )
elseif (APPLE)
    target_include_directories(external_openh264 SYSTEM
    INTERFACE
        ${libs_loc}/local/include
    )
    set_target_properties(external_openh264 PROPERTIES
        IMPORTED_LOCATION ${libs_loc}/local/lib/libopenh264.a
    )
else()
    find_library(DESKTOP_APP_OPENH264_LIBRARIES libopenh264.a REQUIRED)
    set_target_properties(external_openh264 PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_OPENH264_LIBRARIES}"
    )
endif()
