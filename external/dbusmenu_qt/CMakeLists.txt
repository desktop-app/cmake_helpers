# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_dbusmenu_qt INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_dbusmenu_qt ALIAS external_dbusmenu_qt)

    find_package(dbusmenu-qt5 REQUIRED)
    target_link_libraries(external_dbusmenu_qt INTERFACE dbusmenu-qt5)
else()
    add_library(external_dbusmenu_qt STATIC)
    add_library(desktop-app::external_dbusmenu_qt ALIAS external_dbusmenu_qt)
    init_target(external_dbusmenu_qt "(external)")

    set(dbusmenu_qt_VERSION_MAJOR 0)
    set(dbusmenu_qt_VERSION_MINOR 9)
    set(dbusmenu_qt_VERSION_PATCH 2)

    set(dbusmenu_qt_loc ${third_party_loc}/libdbusmenu-qt/src)

    set_target_properties(external_dbusmenu_qt PROPERTIES AUTOMOC ON)

    # QIcon::name was introduced in Qt 4.7.
    set(HAVE_QICON_NAME ON)

    configure_file(${dbusmenu_qt_loc}/dbusmenu_config.h.in "${CMAKE_CURRENT_BINARY_DIR}/dbusmenu_config.h" @ONLY)
    configure_file(${dbusmenu_qt_loc}/dbusmenu_version.h.in "${CMAKE_CURRENT_BINARY_DIR}/dbusmenu_version.h")

    nice_target_sources(external_dbusmenu_qt ${dbusmenu_qt_loc}
    PRIVATE
        dbusmenu_p.cpp
        dbusmenuexporter.cpp
        dbusmenuexporterdbus_p.cpp
        dbusmenuimporter.cpp
        dbusmenutypes_p.cpp
        dbusmenushortcut_p.cpp
        utils.cpp
    )

    qt5_add_dbus_adaptor(dbusmenu_adaptor
        ${dbusmenu_qt_loc}/com.canonical.dbusmenu.xml
        ${dbusmenu_qt_loc}/dbusmenuexporterdbus_p.h
        DBusMenuExporterDBus
    )

    target_sources(external_dbusmenu_qt PRIVATE ${dbusmenu_adaptor})

    target_include_directories(external_dbusmenu_qt
    PUBLIC
        ${dbusmenu_qt_loc}
        "${CMAKE_CURRENT_BINARY_DIR}"
    )

    target_link_libraries(external_dbusmenu_qt
    PRIVATE
        desktop-app::external_qt
    )
endif()
