# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_angle INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_angle ALIAS external_angle)

if (WIN32)
    target_include_directories(external_angle SYSTEM
    INTERFACE
        ${libs_loc}/tg_angle/include
    )
    target_link_libraries(external_angle
    INTERFACE
        ${libs_loc}/tg_angle/out/$<IF:$<CONFIG:Debug>,Debug,Release>/tg_angle.lib
        dxguid.lib
    )
    target_link_libraries(external_angle
    INTERFACE
        desktop-app::win_directx_helper
    )
    target_compile_definitions(external_angle
    INTERFACE
        KHRONOS_STATIC
    )
endif()
