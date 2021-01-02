# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_kwayland INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_kwayland ALIAS external_kwayland)

    find_package(KF5Wayland REQUIRED)
    target_link_libraries(external_kwayland INTERFACE KF5::WaylandClient)
else()
    add_library(external_kwayland STATIC IMPORTED GLOBAL)
    add_library(desktop-app::external_kwayland ALIAS external_kwayland)

    find_library(KWAYLAND_LIBRARY libKF5WaylandClient.a)
    set_target_properties(external_kwayland PROPERTIES
        IMPORTED_LOCATION "${KWAYLAND_LIBRARY}"
    )

    target_include_directories(external_kwayland
    INTERFACE
        /usr/local/include/KF5
        /usr/local/include/KF5/KWayland/Client
    )

    target_link_static_libraries(external_kwayland
    INTERFACE
        wayland-client
        ffi
    )

    target_link_libraries(external_kwayland
    INTERFACE
        ${qt_loc}/lib/libQt5Concurrent.a
        desktop-app::external_qt
    )
endif()
