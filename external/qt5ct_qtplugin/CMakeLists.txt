# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY_PLATFORMTHEMES)
    add_library(external_qt5ct_qtplugin INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_qt5ct_qtplugin ALIAS external_qt5ct_qtplugin)
else()
    add_library(external_qt5ct_qtplugin STATIC)
    add_library(desktop-app::external_qt5ct_qtplugin ALIAS external_qt5ct_qtplugin)
    init_target(external_qt5ct_qtplugin "(external)")

    set(qt5ct_loc ${third_party_loc}/qt5ct)
    set(qt5ct_qtplugin_src ${qt5ct_loc}/src/qt5ct-qtplugin)

    set_target_properties(external_qt5ct_qtplugin PROPERTIES AUTOMOC ON)

    nice_target_sources(external_qt5ct_qtplugin ${qt5ct_qtplugin_src}
    PRIVATE
        main.cpp
        qt5ctplatformtheme.cpp
        qt5ctplatformtheme.h
    )

    target_include_directories(external_qt5ct_qtplugin
    PRIVATE
        ${qt5ct_qtplugin_src}
    )

    target_compile_definitions(external_qt5ct_qtplugin
    PRIVATE
        QT_STATICPLUGIN
    )

    target_link_libraries(external_qt5ct_qtplugin
    PRIVATE
        desktop-app::external_qt5ct
        desktop-app::external_qt
    )
endif()
