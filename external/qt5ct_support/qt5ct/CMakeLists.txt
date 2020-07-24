# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY_PLATFORMTHEMES)
    add_library(external_qt5ct INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_qt5ct ALIAS external_qt5ct)
else()
    add_library(external_qt5ct STATIC)
    add_library(desktop-app::external_qt5ct ALIAS external_qt5ct)
    init_target(external_qt5ct "(external)")

    set(qt5ct_loc ${third_party_loc}/qt5ct)
    set(qt5ct_src ${qt5ct_loc}/src/qt5ct)

    set_target_properties(external_qt5ct PROPERTIES AUTOMOC ON)

    nice_target_sources(external_qt5ct ${qt5ct_src}
    PRIVATE
        qt5ct.cpp
        qt5ct.h
    )

    target_include_directories(external_qt5ct
    PRIVATE
        ${qt5ct_src}
    )

    target_include_directories(external_qt5ct
    PUBLIC
        ${qt5ct_loc}/src
    )

    target_link_libraries(external_qt5ct
    PRIVATE
        desktop-app::external_qt
    )
endif()
