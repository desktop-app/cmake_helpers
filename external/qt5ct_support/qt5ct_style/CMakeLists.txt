# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY_PLATFORMTHEMES)
    add_library(external_qt5ct_style INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_qt5ct_style ALIAS external_qt5ct_style)
else()
    add_library(external_qt5ct_style STATIC)
    add_library(desktop-app::external_qt5ct_style ALIAS external_qt5ct_style)
    init_target(external_qt5ct_style "(external)")

    set(qt5ct_loc ${third_party_loc}/qt5ct)
    set(qt5ct_style_src ${qt5ct_loc}/src/qt5ct-style)

    set_target_properties(external_qt5ct_style PROPERTIES AUTOMOC ON)

    nice_target_sources(external_qt5ct_style ${qt5ct_style_src}
    PRIVATE
        plugin.cpp
        qt5ctproxystyle.cpp
        qt5ctproxystyle.h
    )

    target_include_directories(external_qt5ct_style
    PRIVATE
        ${qt5ct_style_src}
    )

    target_compile_definitions(external_qt5ct_style
    PRIVATE
        QT_STATICPLUGIN
    )

    target_link_libraries(external_qt5ct_style
    PRIVATE
        desktop-app::external_qt5ct
        desktop-app::external_qt
    )
endif()
