# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_opus INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_opus ALIAS external_opus)

    find_package(Opus QUIET)
    if (Opus_FOUND)
        target_link_libraries(external_opus INTERFACE Opus::opus)
        return()
    endif()

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(DESKTOP_APP_OPUS IMPORTED_TARGET opus)

    target_link_libraries(external_opus INTERFACE PkgConfig::DESKTOP_APP_OPUS)
    return()
endif()

add_library(external_opus STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_opus ALIAS external_opus)

if (WIN32)
    target_include_directories(external_opus SYSTEM
    INTERFACE
        ${libs_loc}/opus/include
    )
    set(opus_lib_loc ${libs_loc}/opus/out)
    set_target_properties(external_opus PROPERTIES
        IMPORTED_LOCATION "${opus_lib_loc}/Release/opus.lib"
        IMPORTED_LOCATION_DEBUG "${opus_lib_loc}/Debug/opus.lib"
    )
elseif (APPLE)
    target_include_directories(external_opus SYSTEM
    INTERFACE
        ${libs_loc}/local/include/opus
    )
    set_target_properties(external_opus PROPERTIES
        IMPORTED_LOCATION ${libs_loc}/local/lib/libopus.a
    )
else()
    target_include_directories(external_opus SYSTEM
    INTERFACE
        /usr/local/include/opus
    )
    find_library(DESKTOP_APP_OPUS_LIBRARIES libopus.a REQUIRED)
    set_target_properties(external_opus PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_OPUS_LIBRARIES}"
    )
endif()
