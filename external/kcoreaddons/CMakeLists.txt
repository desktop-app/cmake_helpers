# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (NOT LINUX)
    add_library(external_kcoreaddons INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_kcoreaddons ALIAS external_kcoreaddons)

    target_include_directories(external_kcoreaddons SYSTEM
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(external_kcoreaddons
    INTERFACE
        desktop-app::external_qt
    )

    return()
endif()

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_kcoreaddons INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_kcoreaddons ALIAS external_kcoreaddons)

    find_package(KF5CoreAddons REQUIRED)
    target_link_libraries(external_kcoreaddons INTERFACE KF5::CoreAddons)
    return()
endif()

add_library(external_kcoreaddons STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_kcoreaddons ALIAS external_kcoreaddons)

find_library(DESKTOP_APP_KCOREADDONS_LIBRARIES libKF5CoreAddons.a REQUIRED)
set_target_properties(external_kcoreaddons PROPERTIES
    IMPORTED_LOCATION "${DESKTOP_APP_KCOREADDONS_LIBRARIES}"
)

target_include_directories(external_kcoreaddons SYSTEM
INTERFACE
    /usr/local/include/KF${QT_VERSION_MAJOR}/KCoreAddons
)

target_link_libraries(external_kcoreaddons
INTERFACE
    desktop-app::external_qt
)
