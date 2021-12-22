# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_kwayland INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_kwayland ALIAS external_kwayland)

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_QT6)
    find_package(KF5Wayland)

    if (KF5Wayland_FOUND)
        target_link_libraries(external_kwayland INTERFACE KF5::WaylandClient)
        return()
    endif()
endif()

add_library(external_kwayland_bundled STATIC IMPORTED)
set(kwayland_loc ${third_party_loc}/kwayland)

set(kwayland_patch_command :)
if (DESKTOP_APP_QT6)
    set(kwayland_patch_command patch -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/../../patches/kwayland-qt6.patch)
endif()

include(ExternalProject)
ExternalProject_Add(kwayland
    SOURCE_DIR ${kwayland_loc}
    CMAKE_GENERATOR Ninja
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_SHARED_LIBS=OFF
        -DBUILD_TESTING=OFF
    PATCH_COMMAND ${kwayland_patch_command}
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --target KF5WaylandClient
    STEP_TARGETS build
    EXCLUDE_FROM_ALL TRUE
    BUILD_BYPRODUCTS <BINARY_DIR>/lib/libKF5WaylandClient.a
)

ExternalProject_Get_property(kwayland BINARY_DIR)
file(MAKE_DIRECTORY "${BINARY_DIR}/src/client")

set_target_properties(external_kwayland_bundled PROPERTIES
    IMPORTED_LOCATION "${BINARY_DIR}/lib/libKF5WaylandClient.a"
)

target_include_directories(external_kwayland_bundled SYSTEM
INTERFACE
    ${kwayland_loc}/src/client
    ${BINARY_DIR}/src/client
)

target_link_libraries(external_kwayland_bundled
INTERFACE
    desktop-app::external_qt
)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WAYLAND_CLIENT REQUIRED IMPORTED_TARGET wayland-client)
    target_link_libraries(external_kwayland_bundled INTERFACE PkgConfig::WAYLAND_CLIENT)
else()
    target_link_libraries(external_kwayland_bundled
    INTERFACE
        desktop-app::linux_wayland_helper
        $<TARGET_FILE:desktop-app::linux_wayland_helper>
    )
endif()

add_dependencies(external_kwayland_bundled kwayland-build)

target_link_libraries(external_kwayland
INTERFACE
    external_kwayland_bundled
)
