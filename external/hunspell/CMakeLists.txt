# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_hunspell INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_hunspell ALIAS external_hunspell)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(HUNSPELL hunspell)
    target_include_directories(external_hunspell INTERFACE ${HUNSPELL_INCLUDE_DIRS})
else()
    target_include_directories(external_hunspell SYSTEM
    INTERFACE
        ${libs_loc}/hunspell/src/hunspell
    )
endif()

if (DESKTOP_APP_USE_PACKAGED)
    target_link_libraries(external_hunspell INTERFACE ${HUNSPELL_LIBRARIES})
else()
    target_link_static_libraries(external_hunspell INTERFACE hunspell-1.7)
endif()
