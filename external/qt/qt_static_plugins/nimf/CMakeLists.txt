# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt_static_plugins_nimf STATIC)
add_library(desktop-app::external_qt_static_plugins_nimf ALIAS external_qt_static_plugins_nimf)
init_target(external_qt_static_plugins_nimf "(external)")

set(nimf_loc ${third_party_loc}/nimf)
set(libnimf_src ${nimf_loc}/libnimf)
set(nimf_qt5_src ${nimf_loc}/modules/clients/qt5)

set_target_properties(external_qt_static_plugins_nimf PROPERTIES AUTOMOC ON)

nice_target_sources(external_qt_static_plugins_nimf ${nimf_qt5_src}
PRIVATE
    im-nimf-qt5.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DESKTOP_APP_GIO REQUIRED gio-2.0)

target_include_directories(external_qt_static_plugins_nimf
PRIVATE
    ${nimf_qt5_src}
    ${libnimf_src}
    ${DESKTOP_APP_GIO_INCLUDE_DIRS}
)

target_compile_definitions(external_qt_static_plugins_nimf
PRIVATE
    QT_STATICPLUGIN
    QT_NO_KEYWORDS
    G_LOG_DOMAIN="nimf"
    NIMF_COMPILATION
    USE_DLFCN
)

target_link_libraries(external_qt_static_plugins_nimf
PRIVATE
    desktop-app::external_qt
)

add_library(external_qt_static_plugins_nimf_init OBJECT)
add_library(desktop-app::external_qt_static_plugins_nimf_init ALIAS external_qt_static_plugins_nimf_init)
init_target(external_qt_static_plugins_nimf_init "(external)")

nice_target_sources(external_qt_static_plugins_nimf_init ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    init.cpp
)

target_link_libraries(external_qt_static_plugins_nimf_init
PRIVATE
    desktop-app::external_qt
)

target_link_libraries(external_qt_static_plugins_nimf
INTERFACE
    external_qt_static_plugins_nimf_init
    $<TARGET_OBJECTS:external_qt_static_plugins_nimf_init>
)
