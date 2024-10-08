# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_fcitx5_qt INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_fcitx5_qt ALIAS external_fcitx5_qt)
    return()
endif()

add_library(external_fcitx5_qt STATIC)
add_library(desktop-app::external_fcitx5_qt ALIAS external_fcitx5_qt)
init_target(external_fcitx5_qt "(external)")

set(fcitx5_qt_loc ${third_party_loc}/fcitx5-qt)
set(fcitx5_qt_src ${fcitx5_qt_loc}/qt${QT_VERSION_MAJOR}/platforminputcontext)

set_target_properties(external_fcitx5_qt PROPERTIES AUTOMOC ON)
set(FCITX5_QT_EXTRA_PLUGIN_NAME "\"fcitx\",")

configure_file("${fcitx5_qt_src}/fcitx5.json.in" "${CMAKE_CURRENT_BINARY_DIR}/fcitx5.json")

nice_target_sources(external_fcitx5_qt ${fcitx5_qt_src}
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

target_include_directories(external_fcitx5_qt
PRIVATE
    ${fcitx5_qt_src}
    ${fcitx5_qt_loc}/common
)

target_compile_definitions(external_fcitx5_qt
PRIVATE
    QT_STATICPLUGIN
    FCITX_PLUGIN_DATA_FILE_PATH="${CMAKE_CURRENT_BINARY_DIR}/fcitx5.json"
)

add_subdirectory(fcitx5_qt_dbusaddons)
target_link_libraries(external_fcitx5_qt
PRIVATE
    desktop-app::external_fcitx5_qt_dbusaddons
    desktop-app::external_qt
)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XKBCOMMON REQUIRED IMPORTED_TARGET xkbcommon)

    target_link_libraries(external_fcitx5_qt
    PRIVATE
        PkgConfig::XKBCOMMON
    )
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)

    target_include_directories(external_fcitx5_qt SYSTEM
    PUBLIC
        ${XKBCOMMON_INCLUDE_DIRS}
    )

    target_link_static_libraries(external_fcitx5_qt
    PRIVATE
        xkbcommon
    )
endif()

if (NOT DESKTOP_APP_DISABLE_X11_INTEGRATION)
    target_compile_definitions(external_fcitx5_qt
    PRIVATE
        ENABLE_X11
    )

    target_link_libraries(external_fcitx5_qt
    PRIVATE
        desktop-app::external_xcb
    )
endif()

add_library(external_fcitx5_qt_init OBJECT)
add_library(desktop-app::external_fcitx5_qt_init ALIAS external_fcitx5_qt_init)
init_target(external_fcitx5_qt_init "(external)")

nice_target_sources(external_fcitx5_qt_init ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    init.cpp
)

target_link_libraries(external_fcitx5_qt_init
PRIVATE
    desktop-app::external_qt
)

target_link_libraries(external_fcitx5_qt
INTERFACE
    external_fcitx5_qt_init
    $<TARGET_OBJECTS:external_fcitx5_qt_init>
)
