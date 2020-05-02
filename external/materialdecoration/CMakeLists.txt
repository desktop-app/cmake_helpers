# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_materialdecoration INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_materialdecoration ALIAS external_materialdecoration)
else()
    add_library(external_materialdecoration STATIC)
    add_library(desktop-app::external_materialdecoration ALIAS external_materialdecoration)
    init_target(external_materialdecoration "(external)")

    set(materialdecoration_loc ${third_party_loc}/materialdecoration)
    set(materialdecoration_src ${materialdecoration_loc}/src/plugins/decorations/material)

    set_target_properties(external_materialdecoration PROPERTIES AUTOMOC ON)

    nice_target_sources(external_materialdecoration ${materialdecoration_src}
    PRIVATE
        materialdecoration.cpp
        materialdecoration.h
        plugin.cpp
    )

    target_include_directories(external_materialdecoration
    PRIVATE
        ${materialdecoration_src}
    )

    target_compile_definitions(external_materialdecoration
    PRIVATE
        QT_STATICPLUGIN
    )

    target_link_libraries(external_materialdecoration
    PRIVATE
        desktop-app::external_qt
    )

    if (DESKTOP_APP_USE_PACKAGED)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(WAYLAND_CLIENT REQUIRED IMPORTED_TARGET wayland-client)

        target_link_libraries(external_materialdecoration
        PRIVATE
            PkgConfig::WAYLAND_CLIENT
        )
    else()
        target_link_static_libraries(external_materialdecoration
        PRIVATE
            wayland-client
        )
    endif()
endif()
