# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt_static_plugins_fcitx5_dbusaddons STATIC)
add_library(desktop-app::external_qt_static_plugins_fcitx5_dbusaddons ALIAS external_qt_static_plugins_fcitx5_dbusaddons)
init_target(external_qt_static_plugins_fcitx5_dbusaddons "(external)")

set(fcitx5_qt_loc ${third_party_loc}/fcitx5-qt)
set(fcitx5_qt_dbusaddons_src ${fcitx5_qt_loc}/qt${QT_VERSION_MAJOR}/dbusaddons)

set_target_properties(external_qt_static_plugins_fcitx5_dbusaddons PROPERTIES AUTOMOC ON)

nice_target_sources(external_qt_static_plugins_fcitx5_dbusaddons ${fcitx5_qt_dbusaddons_src}
PRIVATE
    fcitxqtwatcher.cpp
    fcitxqtwatcher.h
    fcitxqtdbustypes.cpp
    fcitxqtdbustypes.h
    fcitxqtinputcontextproxy.cpp
    fcitxqtinputcontextproxy.h
    fcitxqtinputcontextproxyimpl.cpp
    fcitxqtinputmethodproxy.cpp
    fcitxqtinputmethodproxy.h
    fcitxqtcontrollerproxy.cpp
    fcitxqtcontrollerproxy.h
)

include(GenerateExportHeader)
generate_export_header(external_qt_static_plugins_fcitx5_dbusaddons BASE_NAME Fcitx5Qt${QT_VERSION_MAJOR}DBusAddons)

target_include_directories(external_qt_static_plugins_fcitx5_dbusaddons
PUBLIC
    ${fcitx5_qt_dbusaddons_src}
    "${CMAKE_CURRENT_BINARY_DIR}"
)

if (QT_VERSION_MAJOR EQUAL 5)
    target_compile_definitions(external_qt_static_plugins_fcitx5_dbusaddons
    PRIVATE
        FCITX5QT5DBUSADDONS_STATIC_DEFINE
    )
endif()

target_link_libraries(external_qt_static_plugins_fcitx5_dbusaddons
PRIVATE
    desktop-app::external_qt
)
