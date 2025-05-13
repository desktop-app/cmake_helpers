# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_hunspell INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_hunspell ALIAS external_hunspell)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig)
    if (PkgConfig_FOUND)
        pkg_check_modules(DESKTOP_APP_HUNSPELL IMPORTED_TARGET hunspell)
    endif()

    if (DESKTOP_APP_HUNSPELL_FOUND)
        target_link_libraries(external_hunspell INTERFACE PkgConfig::DESKTOP_APP_HUNSPELL)
        return()
    endif()
endif()

add_library(external_hunspell_bundled STATIC)
init_target(external_hunspell_bundled "(external)")

set(hunspell_loc ${third_party_loc}/hunspell)
set(hunspell_src ${hunspell_loc}/src)

nice_target_sources(external_hunspell_bundled ${hunspell_src}
PRIVATE
    hunspell/affentry.cxx
    hunspell/affentry.hxx
    hunspell/affixmgr.cxx
    hunspell/affixmgr.hxx
    hunspell/atypes.hxx
    hunspell/baseaffix.hxx
    hunspell/csutil.cxx
    hunspell/csutil.hxx
    hunspell/filemgr.cxx
    hunspell/filemgr.hxx
    hunspell/hashmgr.cxx
    hunspell/hashmgr.hxx
    hunspell/htypes.hxx
    hunspell/hunspell.cxx
    hunspell/hunspell.h
    hunspell/hunspell.hxx
    hunspell/hunvisapi.h
    hunspell/hunzip.cxx
    hunspell/hunzip.hxx
    hunspell/phonet.cxx
    hunspell/phonet.hxx
    hunspell/replist.cxx
    hunspell/replist.hxx
    hunspell/suggestmgr.cxx
    hunspell/suggestmgr.hxx
    hunspell/utf_info.hxx
)

target_include_directories(external_hunspell_bundled
PUBLIC
    ${hunspell_src}
    ${hunspell_src}/hunspell
)

target_compile_definitions(external_hunspell_bundled
PUBLIC
    HUNSPELL_STATIC
    MAXSUGGESTION=5
)

if (WIN32)
    target_compile_definitions(external_hunspell_bundled
    PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )
endif()

target_link_libraries(external_hunspell
INTERFACE
    external_hunspell_bundled
)
