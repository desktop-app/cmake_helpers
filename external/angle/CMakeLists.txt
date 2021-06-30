# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_angle INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_angle ALIAS external_angle)

if (WIN32)
    target_include_directories(external_angle
    INTERFACE
        ${libs_loc}/angle/include
    )
    set(angle_libs_loc ${libs_loc}/angle/out/$<IF:$<CONFIG:Debug>,Debug,Release>/obj)
    target_link_libraries(external_angle
    INTERFACE
        ${angle_libs_loc}/libANGLE_static.lib
        ${angle_libs_loc}/libGLESv2_static.lib
        ${angle_libs_loc}/libEGL_static.lib
        dxguid.lib
    )
    target_link_libraries(external_angle
    INTERFACE
        desktop-app::win_directx_helper
    )
endif()
