# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_hime_qt INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_hime_qt ALIAS external_hime_qt)
    return()
endif()

add_library(external_hime_qt STATIC)
add_library(desktop-app::external_hime_qt ALIAS external_hime_qt)
init_target(external_hime_qt "(external)")

set(hime_loc ${third_party_loc}/hime)
set(hime_qt_src ${hime_loc}/src/qt5-im)

set_target_properties(external_hime_qt PROPERTIES AUTOMOC ON)

nice_target_sources(external_hime_qt ${hime_qt_src}
PRIVATE
    hime-imcontext-qt.cpp
    hime-imcontext-qt.h
    hime-qt.cpp
    hime-qt.h
)

target_include_directories(external_hime_qt
PRIVATE
    ${hime_qt_src}
)

target_compile_definitions(external_hime_qt
PRIVATE
    QT_STATICPLUGIN
)

add_subdirectory(hime_im_client)
target_link_libraries(external_hime_qt
PRIVATE
    desktop-app::external_hime_im_client
    desktop-app::external_qt
)
