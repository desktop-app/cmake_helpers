# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_jpeg INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_jpeg ALIAS external_jpeg)

    find_package(JPEG REQUIRED)
    target_link_libraries(external_jpeg INTERFACE JPEG::JPEG)
    return()
endif()

add_library(external_jpeg STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_jpeg ALIAS external_jpeg)

if (WIN32)
    set(jpeg_lib_loc ${libs_loc}/mozjpeg)
    target_include_directories(external_jpeg SYSTEM INTERFACE ${jpeg_lib_loc})
    set_target_properties(external_jpeg PROPERTIES
        IMPORTED_LOCATION "${jpeg_lib_loc}/Release/jpeg-static.lib"
        IMPORTED_LOCATION_DEBUG "${jpeg_lib_loc}/Debug/jpeg-static.lib"
    )
elseif (APPLE)
    set_target_properties(external_jpeg PROPERTIES
        IMPORTED_LOCATION ${libs_loc}/local/lib/libjpeg.a
    )
else()
    find_library(DESKTOP_APP_JPEG_LIBRARIES libjpeg.a REQUIRED)
    set_target_properties(external_jpeg PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_JPEG_LIBRARIES}"
    )
    target_link_static_libraries(external_jpeg INTERFACE hwy)
endif()
