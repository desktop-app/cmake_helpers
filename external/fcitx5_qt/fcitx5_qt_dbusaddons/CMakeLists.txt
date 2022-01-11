# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_fcitx5_qt_dbusaddons INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_fcitx5_qt_dbusaddons ALIAS external_fcitx5_qt_dbusaddons)
    return()
endif()

add_library(external_fcitx5_qt_dbusaddons STATIC)
add_library(desktop-app::external_fcitx5_qt_dbusaddons ALIAS external_fcitx5_qt_dbusaddons)
init_target(external_fcitx5_qt_dbusaddons "(external)")

set(fcitx5_qt_loc ${third_party_loc}/fcitx5-qt)

if (DESKTOP_APP_QT6)
    set(fcitx5_qt_dbusaddons_src ${fcitx5_qt_loc}/qt6/dbusaddons)
else()
    set(fcitx5_qt_dbusaddons_src ${fcitx5_qt_loc}/qt5/dbusaddons)
endif()

set_target_properties(external_fcitx5_qt_dbusaddons PROPERTIES AUTOMOC ON)

nice_target_sources(external_fcitx5_qt_dbusaddons ${fcitx5_qt_dbusaddons_src}
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

if (DESKTOP_APP_QT6)
    generate_export_header(external_fcitx5_qt_dbusaddons BASE_NAME Fcitx5Qt6DBusAddons)
else()
    generate_export_header(external_fcitx5_qt_dbusaddons BASE_NAME Fcitx5Qt5DBusAddons)
endif()

target_include_directories(external_fcitx5_qt_dbusaddons
PUBLIC
    ${fcitx5_qt_dbusaddons_src}
    "${CMAKE_CURRENT_BINARY_DIR}"
)

if (NOT DESKTOP_APP_QT6)
    target_compile_definitions(external_fcitx5_qt_dbusaddons
    PRIVATE
        FCITX5QT5DBUSADDONS_STATIC_DEFINE
    )
endif()

target_link_libraries(external_fcitx5_qt_dbusaddons
PRIVATE
    desktop-app::external_qt
)
