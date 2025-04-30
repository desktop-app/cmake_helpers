# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt_static_plugins_hime STATIC)
add_library(desktop-app::external_qt_static_plugins_hime ALIAS external_qt_static_plugins_hime)
init_target(external_qt_static_plugins_hime "(external)")

set(hime_loc ${third_party_loc}/hime)
set(hime_qt_src ${hime_loc}/src/qt5-im)

set_target_properties(external_qt_static_plugins_hime PROPERTIES AUTOMOC ON)

nice_target_sources(external_qt_static_plugins_hime ${hime_qt_src}
PRIVATE
    hime-imcontext-qt.cpp
    hime-imcontext-qt.h
    hime-qt.cpp
    hime-qt.h
)

target_include_directories(external_qt_static_plugins_hime
PRIVATE
    ${hime_qt_src}
)

target_compile_definitions(external_qt_static_plugins_hime
PRIVATE
    QT_STATICPLUGIN
)

add_subdirectory(hime_im_client)
target_link_libraries(external_qt_static_plugins_hime
PRIVATE
    desktop-app::external_qt_static_plugins_hime_im_client
    desktop-app::external_qt
)

add_library(external_qt_static_plugins_hime_init OBJECT)
add_library(desktop-app::external_qt_static_plugins_hime_init ALIAS external_qt_static_plugins_hime_init)
init_target(external_qt_static_plugins_hime_init "(external)")

nice_target_sources(external_qt_static_plugins_hime_init ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    init.cpp
)

target_link_libraries(external_qt_static_plugins_hime_init
PRIVATE
    desktop-app::external_qt
)

target_link_libraries(external_qt_static_plugins_hime
INTERFACE
    external_qt_static_plugins_hime_init
    $<TARGET_OBJECTS:external_qt_static_plugins_hime_init>
)
