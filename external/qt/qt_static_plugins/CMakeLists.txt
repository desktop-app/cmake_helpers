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

if (DESKTOP_APP_USE_PACKAGED)
    target_compile_definitions(external_qt_static_plugins
    PRIVATE
        QT_STATIC_PLUGINS_USE_PACKAGED
    )
endif()

if (DESKTOP_APP_DISABLE_QT_PLUGINS)
    return()
endif()

add_checked_subdirectory(kimageformats)
target_link_libraries(external_qt_static_plugins
PUBLIC
    desktop-app::external_qt_static_plugins_kimageformats
)

if (LINUX)
    add_checked_subdirectory(nimf)
    target_link_libraries(external_qt_static_plugins
    PUBLIC
        desktop-app::external_qt_static_plugins_nimf
    )

    if (TARGET Qt::DBus)
        add_checked_subdirectory(fcitx5)
        target_link_libraries(external_qt_static_plugins
        PUBLIC
            desktop-app::external_qt_static_plugins_fcitx5
        )
    endif()

    if (NOT DESKTOP_APP_DISABLE_X11_INTEGRATION)
        add_checked_subdirectory(hime)
        target_link_libraries(external_qt_static_plugins
        PUBLIC
            desktop-app::external_qt_static_plugins_hime
        )
    endif()
endif()
