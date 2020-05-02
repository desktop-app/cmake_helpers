# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_fcitx_qt5 INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_fcitx_qt5 ALIAS external_fcitx_qt5)
else()
    add_library(external_fcitx_qt5 STATIC)
    add_library(desktop-app::external_fcitx_qt5 ALIAS external_fcitx_qt5)
    init_target(external_fcitx_qt5 "(external)")

    set(fcitx_qt5_loc ${third_party_loc}/fcitx-qt5)
    set(fcitx_qt5_src ${fcitx_qt5_loc}/platforminputcontext)

    set_target_properties(external_fcitx_qt5 PROPERTIES AUTOMOC ON)

    nice_target_sources(external_fcitx_qt5 ${fcitx_qt5_src}
    PRIVATE
        fcitxinputcontextproxy.cpp
        fcitxqtdbustypes.cpp
        fcitxwatcher.cpp
        qfcitxplatforminputcontext.cpp
        main.cpp
        qtkey.cpp
    )

    set_source_files_properties(${fcitx_qt5_src}/org.fcitx.Fcitx.InputContext.xml PROPERTIES
    INCLUDE ${fcitx_qt5_src}/fcitxqtdbustypes.h)
    set_source_files_properties(${fcitx_qt5_src}/org.fcitx.Fcitx.InputContext1.xml PROPERTIES
    INCLUDE ${fcitx_qt5_src}/fcitxqtdbustypes.h)
    set_source_files_properties(${fcitx_qt5_src}/org.fcitx.Fcitx.InputMethod.xml PROPERTIES
    INCLUDE ${fcitx_qt5_src}/fcitxqtdbustypes.h)
    set_source_files_properties(${fcitx_qt5_src}/org.fcitx.Fcitx.InputMethod1.xml PROPERTIES
    INCLUDE ${fcitx_qt5_src}/fcitxqtdbustypes.h)

    qt5_add_dbus_interface(fcitx_qt5_adaptors ${fcitx_qt5_src}/org.fcitx.Fcitx.InputContext.xml inputcontextproxy)
    qt5_add_dbus_interface(fcitx_qt5_adaptors ${fcitx_qt5_src}/org.fcitx.Fcitx.InputContext1.xml inputcontext1proxy)
    qt5_add_dbus_interface(fcitx_qt5_adaptors ${fcitx_qt5_src}/org.fcitx.Fcitx.InputMethod.xml inputmethodproxy)
    qt5_add_dbus_interface(fcitx_qt5_adaptors ${fcitx_qt5_src}/org.fcitx.Fcitx.InputMethod1.xml inputmethod1proxy)

    target_sources(external_fcitx_qt5 PRIVATE ${fcitx_qt5_adaptors})

    target_include_directories(external_fcitx_qt5
    PRIVATE
        ${fcitx_qt5_src}
        "${CMAKE_CURRENT_BINARY_DIR}"
    )

    target_compile_definitions(external_fcitx_qt5
    PRIVATE
        QT_STATICPLUGIN
    )

    target_link_libraries(external_fcitx_qt5
    PRIVATE
        desktop-app::external_qt
    )

    if (DESKTOP_APP_USE_PACKAGED)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(XKBCOMMON REQUIRED IMPORTED_TARGET xkbcommon)

        target_link_libraries(external_fcitx_qt5
        PRIVATE
            PkgConfig::XKBCOMMON
        )
    else()
        target_link_static_libraries(external_fcitx_qt5
        PRIVATE
            xkbcommon
        )
    endif()
endif()
