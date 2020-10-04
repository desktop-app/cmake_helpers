# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_nimf_qt5 INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_nimf_qt5 ALIAS external_nimf_qt5)
else()
    add_library(external_nimf_qt5 STATIC)
    add_library(desktop-app::external_nimf_qt5 ALIAS external_nimf_qt5)
    init_target(external_nimf_qt5 "(external)")

    set(nimf_loc ${third_party_loc}/nimf)
    set(libnimf_src ${nimf_loc}/libnimf)
    set(nimf_qt5_src ${nimf_loc}/modules/clients/qt5)

    set_target_properties(external_nimf_qt5 PROPERTIES AUTOMOC ON)

    nice_target_sources(external_nimf_qt5 ${nimf_qt5_src}
    PRIVATE
        im-nimf-qt5.cpp
    )

    target_include_directories(external_nimf_qt5
    PRIVATE
        ${nimf_qt5_src}
        ${libnimf_src}
    )

    target_compile_definitions(external_nimf_qt5
    PRIVATE
        QT_STATICPLUGIN
        QT_NO_KEYWORDS
        G_LOG_DOMAIN="nimf"
        NIMF_COMPILATION
        USE_DLFCN
    )

    target_link_libraries(external_nimf_qt5
    PRIVATE
        desktop-app::external_qt
        desktop-app::external_glib
        ${CMAKE_DL_LIBS}
    )
endif()
