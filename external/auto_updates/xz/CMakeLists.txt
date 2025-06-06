# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_xz INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_xz ALIAS external_xz)

if (DESKTOP_APP_USE_PACKAGED OR LINUX)
    find_package(LibLZMA REQUIRED)
    target_link_libraries(external_xz INTERFACE LibLZMA::LibLZMA)
elseif (APPLE)
    target_include_directories(external_xz SYSTEM
    INTERFACE
        ${libs_loc}/local/include
    )
    target_link_libraries(external_xz
    INTERFACE
        ${libs_loc}/local/lib/liblzma.a
    )
endif()
