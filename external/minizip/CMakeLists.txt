# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_minizip INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_minizip ALIAS external_minizip)

    find_package(minizip QUIET COMPONENTS shared)
    if (minizip_FOUND)
        target_link_libraries(external_minizip INTERFACE MINIZIP::minizip)
        return()
    endif()

    find_package(minizip QUIET COMPONENTS static)
    if (minizip_FOUND)
        target_link_libraries(external_minizip INTERFACE MINIZIP::minizipstatic)
        return()
    endif()

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(DESKTOP_APP_MINIZIP REQUIRED IMPORTED_TARGET minizip)
    target_link_libraries(external_minizip INTERFACE PkgConfig::DESKTOP_APP_MINIZIP)
    return()
endif()

add_library(external_minizip STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_minizip ALIAS external_minizip)

if (WIN32)
    target_include_directories(external_minizip SYSTEM
    INTERFACE
        ${libs_loc}/zlib/contrib
    )
    set(minizip_lib_loc ${libs_loc}/zlib/contrib/minizip)
    set_target_properties(external_minizip PROPERTIES
        IMPORTED_LOCATION "${minizip_lib_loc}/Release/libminizips.lib"
        IMPORTED_LOCATION_DEBUG "${minizip_lib_loc}/Debug/libminizipsd.lib"
    )
elseif (APPLE)
    set_target_properties(external_minizip PROPERTIES
        IMPORTED_LOCATION ${libs_loc}/local/lib/libminizip.a
    )
else()
    find_library(DESKTOP_APP_MINIZIP_LIBRARIES libminizip.a REQUIRED)
    set_target_properties(external_minizip PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_MINIZIP_LIBRARIES}"
    )
endif()

target_link_libraries(external_minizip
INTERFACE
    desktop-app::external_zlib
)
