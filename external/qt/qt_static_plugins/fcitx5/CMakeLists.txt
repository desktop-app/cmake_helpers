# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt_static_plugins_fcitx5 STATIC)
add_library(desktop-app::external_qt_static_plugins_fcitx5 ALIAS external_qt_static_plugins_fcitx5)
init_target(external_qt_static_plugins_fcitx5 "(external)")

set(fcitx5_qt_loc ${third_party_loc}/fcitx5-qt)
set(fcitx5_qt_src ${fcitx5_qt_loc}/qt${QT_VERSION_MAJOR}/platforminputcontext)

set_target_properties(external_qt_static_plugins_fcitx5 PROPERTIES AUTOMOC ON)
set(FCITX5_QT_EXTRA_PLUGIN_NAME "\"fcitx\",")

configure_file("${fcitx5_qt_src}/fcitx5.json.in" "${CMAKE_CURRENT_BINARY_DIR}/fcitx5.json")

nice_target_sources(external_qt_static_plugins_fcitx5 ${fcitx5_qt_src}
PRIVATE
    fcitx4watcher.cpp
    fcitx4inputcontextproxy.cpp
    fcitx4inputcontextproxyimpl.cpp
    fcitx4inputmethodproxy.cpp
    hybridinputcontext.cpp
    qfcitxplatforminputcontext.cpp
    fcitxcandidatewindow.cpp
    fcitxtheme.cpp
    font.cpp
    qtkey.cpp
    main.cpp
)

target_include_directories(external_qt_static_plugins_fcitx5
PRIVATE
    ${fcitx5_qt_src}
    ${fcitx5_qt_loc}/common
)

target_compile_definitions(external_qt_static_plugins_fcitx5
PRIVATE
    QT_STATICPLUGIN
    FCITX_PLUGIN_DATA_FILE_PATH="${CMAKE_CURRENT_BINARY_DIR}/fcitx5.json"
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DESKTOP_APP_XKBCOMMON REQUIRED IMPORTED_TARGET xkbcommon)

add_subdirectory(fcitx5_qt_dbusaddons)
target_link_libraries(external_qt_static_plugins_fcitx5
PRIVATE
    desktop-app::external_qt_static_plugins_fcitx5_dbusaddons
    desktop-app::external_qt
    PkgConfig::DESKTOP_APP_XKBCOMMON
)

if (NOT DESKTOP_APP_DISABLE_X11_INTEGRATION)
    target_compile_definitions(external_qt_static_plugins_fcitx5
    PRIVATE
        ENABLE_X11
    )

    target_link_libraries(external_qt_static_plugins_fcitx5
    PRIVATE
        desktop-app::external_xcb
    )
endif()

add_library(external_qt_static_plugins_fcitx5_init OBJECT)
add_library(desktop-app::external_qt_static_plugins_fcitx5_init ALIAS external_qt_static_plugins_fcitx5_init)
init_target(external_qt_static_plugins_fcitx5_init "(external)")

nice_target_sources(external_qt_static_plugins_fcitx5_init ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    init.cpp
)

target_link_libraries(external_qt_static_plugins_fcitx5_init
PRIVATE
    desktop-app::external_qt
)

target_link_libraries(external_qt_static_plugins_fcitx5
INTERFACE
    external_qt_static_plugins_fcitx5_init
    $<TARGET_OBJECTS:external_qt_static_plugins_fcitx5_init>
)
