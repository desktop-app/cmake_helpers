# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_sonnet INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_sonnet ALIAS external_sonnet)

    find_package(KF5Sonnet)

    if (KF5Sonnet_FOUND)
        target_link_libraries(external_sonnet INTERFACE KF5::SonnetCore)
    endif()
else()
    add_subdirectory(parsetrigrams)

    add_library(external_sonnet OBJECT)
    add_library(desktop-app::external_sonnet ALIAS external_sonnet)
    init_target(external_sonnet "(external)")

    get_filename_component(src_loc . REALPATH)
    set(sonnet_loc ${third_party_loc}/sonnet)

    set_target_properties(external_sonnet PROPERTIES AUTOMOC ON AUTORCC ON)

    add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/data/trigrams.map"
                       COMMAND external_parsetrigrams "${sonnet_loc}/data/trigrams" > "${CMAKE_BINARY_DIR}/data/trigrams.map")
    configure_file(${sonnet_loc}/data/trigrams.qrc.in ${CMAKE_BINARY_DIR}/data/trigrams.qrc @ONLY)

    target_sources(external_sonnet
    PRIVATE
        ${CMAKE_BINARY_DIR}/data/trigrams.qrc
        ${sonnet_loc}/src/core/backgroundchecker.cpp
        ${sonnet_loc}/src/core/backgroundchecker.h
        ${sonnet_loc}/src/core/backgroundchecker_p.h
        ${sonnet_loc}/src/core/client.cpp
        ${sonnet_loc}/src/core/client_p.h
        ${sonnet_loc}/src/core/guesslanguage.cpp
        ${sonnet_loc}/src/core/guesslanguage.h
        ${sonnet_loc}/src/core/languagefilter.cpp
        ${sonnet_loc}/src/core/languagefilter_p.h
        ${sonnet_loc}/src/core/loader.cpp
        ${sonnet_loc}/src/core/loader_p.h
        ${sonnet_loc}/src/core/settings.cpp
        ${sonnet_loc}/src/core/settings_p.h
        ${sonnet_loc}/src/core/speller.cpp
        ${sonnet_loc}/src/core/speller.h
        ${sonnet_loc}/src/core/spellerplugin.cpp
        ${sonnet_loc}/src/core/spellerplugin_p.h
        ${sonnet_loc}/src/core/textbreaks.cpp
        ${sonnet_loc}/src/core/textbreaks_p.h
        ${sonnet_loc}/src/core/tokenizer.cpp
        ${sonnet_loc}/src/core/tokenizer_p.h
        ${sonnet_loc}/src/plugins/hunspell/hunspellclient.cpp
        ${sonnet_loc}/src/plugins/hunspell/hunspellclient.h
        ${sonnet_loc}/src/plugins/hunspell/hunspelldict.cpp
        ${sonnet_loc}/src/plugins/hunspell/hunspelldict.h
        ${src_loc}/config/core_debug.cpp
        ${src_loc}/config/core_debug.h
        ${src_loc}/config/hunspelldebug.cpp
        ${src_loc}/config/hunspelldebug.h
    )

    target_compile_definitions(external_sonnet
    PRIVATE
        QT_USE_QSTRINGBUILDER
        SONNET_STATIC
    )

    target_include_directories(external_sonnet
    PUBLIC
        ${sonnet_loc}/src/core
        ${sonnet_loc}/src/plugins/hunspell
        ${src_loc}/config
    )

    target_link_libraries(external_sonnet
    PUBLIC
        desktop-app::external_qt
        desktop-app::external_hunspell
    )
endif()
