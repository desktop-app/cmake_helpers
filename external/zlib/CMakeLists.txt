# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_zlib INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_zlib ALIAS external_zlib)

    find_package(ZLIB REQUIRED)
    target_link_libraries(external_zlib INTERFACE ZLIB::ZLIB)
    return()
endif()

add_library(external_zlib STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_zlib ALIAS external_zlib)

if (WIN32)
    set(zlib_lib_loc ${libs_loc}/zlib)
    target_compile_definitions(external_zlib INTERFACE ZLIB_WINAPI)
    target_include_directories(external_zlib SYSTEM INTERFACE ${zlib_lib_loc})
    set_target_properties(external_zlib PROPERTIES
        IMPORTED_LOCATION "${zlib_lib_loc}/Release/zlibstatic.lib"
        IMPORTED_LOCATION_DEBUG "${zlib_lib_loc}/Debug/zlibstaticd.lib"
    )
elseif (APPLE)
    set_target_properties(external_zlib PROPERTIES
        IMPORTED_LOCATION ${libs_loc}/local/lib/libz.a
    )
else()
    find_library(DESKTOP_APP_ZLIB_LIBRARIES libz.a REQUIRED)
    set_target_properties(external_zlib PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_ZLIB_LIBRARIES}"
    )
endif()
