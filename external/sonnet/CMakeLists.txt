# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND QT_VERSION_MAJOR EQUAL 5)
    add_library(external_sonnet INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_sonnet ALIAS external_sonnet)

    find_package(KF5Sonnet)
    if (KF5Sonnet_FOUND)
        target_link_libraries(external_sonnet INTERFACE KF5::SonnetCore)
        return()
    endif()
endif()

add_library(external_sonnet STATIC)
add_library(desktop-app::external_sonnet ALIAS external_sonnet)
init_target(external_sonnet "(external)")

set(sonnet_loc ${third_party_loc}/sonnet)
set(sonnet_data_loc ${sonnet_loc}/data)
set(sonnet_core_src ${sonnet_loc}/src/core)
set(sonnet_plugins_src ${sonnet_loc}/src/plugins)

set_target_properties(external_sonnet PROPERTIES AUTOMOC ON)

configure_file(${sonnet_data_loc}/trigrams.qrc.in ${CMAKE_CURRENT_BINARY_DIR}/trigrams.qrc @ONLY)
target_sources(external_sonnet PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/trigrams.qrc)

nice_target_sources(external_sonnet ${sonnet_core_src}
PRIVATE
    client.cpp
    client_p.h
    guesslanguage.cpp
    guesslanguage.h
    loader.cpp
    loader_p.h
    settings.cpp
    settings.h
    settingsimpl.cpp
    settingsimpl_p.h
    speller.cpp
    speller.h
    spellerplugin.cpp
    spellerplugin_p.h
    textbreaks.cpp
    textbreaks_p.h
    tokenizer.cpp
    tokenizer_p.h
)

nice_target_sources(external_sonnet ${sonnet_plugins_src}/hunspell
PRIVATE
    hunspellclient.cpp
    hunspellclient.h
    hunspelldict.cpp
    hunspelldict.h
)

if (APPLE)
    nice_target_sources(external_sonnet ${sonnet_plugins_src}/nsspellchecker
    PRIVATE
        nsspellcheckerclient.cpp
        nsspellcheckerclient.h
        nsspellcheckerdict.cpp
        nsspellcheckerdict.h
    )
endif()

target_compile_definitions(external_sonnet
PUBLIC
    SONNETCORE_EXPORT=
    SONNET_STATIC
PRIVATE
    QT_USE_QSTRINGBUILDER
)

target_include_directories(external_sonnet SYSTEM
PUBLIC
    ${sonnet_core_src}
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/public
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/private
)

target_link_libraries(external_sonnet
PRIVATE
    desktop-app::external_qt
    desktop-app::external_hunspell
)

target_prepare_qrc(external_sonnet)

add_executable(external_sonnet_parsetrigrams)
init_target(external_sonnet_parsetrigrams "(external)")

nice_target_sources(external_sonnet_parsetrigrams ${sonnet_data_loc}
PRIVATE
    parsetrigrams.cpp
)

target_link_libraries(external_sonnet_parsetrigrams
PRIVATE
    desktop-app::external_qt
)

include(generate_trigrams.cmake)
generate_trigrams(external_sonnet ${sonnet_data_loc}/trigrams)
