# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY_PLATFORMTHEMES)
    add_library(external_lxqt_qtplugin INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_lxqt_qtplugin ALIAS external_lxqt_qtplugin)
else()
    add_library(external_lxqt_qtplugin STATIC)
    add_library(desktop-app::external_lxqt_qtplugin ALIAS external_lxqt_qtplugin)
    init_target(external_lxqt_qtplugin "(external)")

    set(lxqt_qtplugin_loc ${third_party_loc}/lxqt-qtplugin)
    set(lxqt_qtplugin_src ${lxqt_qtplugin_loc}/src)

    set_target_properties(external_lxqt_qtplugin PROPERTIES AUTOMOC ON)

    nice_target_sources(external_lxqt_qtplugin ${lxqt_qtplugin_src}
    PRIVATE
        main.cpp
        lxqtplatformtheme.cpp
        lxqtplatformtheme.h
        lxqtsystemtrayicon.cpp
        lxqtsystemtrayicon.h
    )

    target_include_directories(external_lxqt_qtplugin
    PRIVATE
        ${lxqt_qtplugin_src}
    )

    target_compile_definitions(external_lxqt_qtplugin
    PRIVATE
        QT_STATICPLUGIN
        QT_NO_FOREACH
        LIB_FM_QT_SONAME="libfm-qt.so.7"
    )

    add_subdirectory(xdgiconloader)
    target_link_libraries(external_lxqt_qtplugin
    PRIVATE
        desktop-app::external_statusnotifieritem
        desktop-app::external_xdgiconloader
        desktop-app::external_qt
    )
endif()
