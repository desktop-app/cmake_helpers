# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt_static_plugins INTERFACE)
add_library(desktop-app::external_qt_static_plugins ALIAS external_qt_static_plugins)

target_link_libraries(external_qt_static_plugins
INTERFACE
    desktop-app::external_qt
)

if (DESKTOP_APP_DISABLE_QT_PLUGINS)
    return()
endif()

add_checked_subdirectory(kimageformats)
target_link_libraries(external_qt_static_plugins
INTERFACE
    desktop-app::external_qt_static_plugins_kimageformats
)

if (LINUX)
    add_checked_subdirectory(hime)
    target_link_libraries(external_qt_static_plugins
    INTERFACE
        desktop-app::external_qt_static_plugins_hime
    )

    add_checked_subdirectory(nimf)
    target_link_libraries(external_qt_static_plugins
    INTERFACE
        desktop-app::external_qt_static_plugins_nimf
    )

    if (TARGET Qt::DBus)
        add_checked_subdirectory(fcitx5)
        target_link_libraries(external_qt_static_plugins
        INTERFACE
            desktop-app::external_qt_static_plugins_fcitx5
        )
    endif()
endif()
