# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt_static_plugins OBJECT)
add_library(desktop-app::external_qt_static_plugins ALIAS external_qt_static_plugins)
init_target(external_qt_static_plugins "(external)")

nice_target_sources(external_qt_static_plugins ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    qt_static_plugins.cpp
)

target_link_libraries(external_qt_static_plugins
PUBLIC
    desktop-app::external_qt
)

if (LINUX)
    target_link_libraries(external_qt_static_plugins
    PUBLIC
        desktop-app::external_nimf_qt5
        desktop-app::external_qt5ct_support
    )

    if (NOT DESKTOP_APP_DISABLE_DBUS_INTEGRATION)
        target_link_libraries(external_qt_static_plugins
        PUBLIC
            desktop-app::external_fcitx_qt5
            desktop-app::external_fcitx5_qt5
            desktop-app::external_hime_qt
        )
    endif()

    if (NOT DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
        target_link_libraries(external_qt_static_plugins
        PUBLIC
            desktop-app::external_materialdecoration
        )
    endif()
endif()
