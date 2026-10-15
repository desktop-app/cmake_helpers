# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

get_filename_component(angle_zlib ${DESKTOP_APP_ANGLE_DIR}/../zlib ABSOLUTE)

foreach(api EGL GLESv2)
    set(HAVE_${api} ON CACHE BOOL "ANGLE provides ${api}")
    set(${api}_INCLUDE_DIR ${DESKTOP_APP_ANGLE_DIR}/include CACHE PATH "ANGLE headers")
    set(${api}_LIBRARY ${DESKTOP_APP_ANGLE_DIR}/out/Release/tg_angle.lib CACHE FILEPATH "ANGLE library")

    if (NOT TARGET ${api}::${api})
        add_library(${api}::${api} STATIC IMPORTED GLOBAL)
        set_target_properties(${api}::${api} PROPERTIES
            IMPORTED_CONFIGURATIONS "Debug;Release"
            IMPORTED_LOCATION ${DESKTOP_APP_ANGLE_DIR}/out/Release/tg_angle.lib
            IMPORTED_LOCATION_DEBUG ${DESKTOP_APP_ANGLE_DIR}/out/Debug/tg_angle.lib
            IMPORTED_LOCATION_RELEASE ${DESKTOP_APP_ANGLE_DIR}/out/Release/tg_angle.lib
            MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
            MAP_IMPORTED_CONFIG_MINSIZEREL Release
            INTERFACE_INCLUDE_DIRECTORIES ${DESKTOP_APP_ANGLE_DIR}/include
        )
        target_link_libraries(${api}::${api}
        INTERFACE
            ${angle_zlib}/$<IF:$<CONFIG:Debug>,Debug/libzsd.lib,Release/libzs.lib>
            d3d9
            dxgi
            dxguid
        )
        if (api STREQUAL "EGL")
            target_link_libraries(${api}::${api} INTERFACE gdi32 user32)
        endif()
    endif()
endforeach()
