# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_hime_im_client INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_hime_im_client ALIAS external_hime_im_client)
else()
    add_library(external_hime_im_client OBJECT)
    add_library(desktop-app::external_hime_im_client ALIAS external_hime_im_client)
    init_target(external_hime_im_client "(external)")

    set(hime_loc ${third_party_loc}/hime)
    set(hime_src ${hime_loc}/src)
    set(hime_im_client_src ${hime_src}/im-client)

    set_target_properties(external_hime_im_client PROPERTIES AUTOMOC ON)

    nice_target_sources(external_hime_im_client ${hime_im_client_src}
    PRIVATE
        hime-im-client-attr.h
        hime-im-client.c
        hime-im-client.h
        hime-protocol.h
        hime-send.c
    )

    nice_target_sources(external_hime_im_client ${hime_src}
    PRIVATE
        hime-conf.c
        util.c
        im-addr.c
        hime-crypt.c
    )

    find_package(PkgConfig REQUIRED)
    pkg_search_module(GTK REQUIRED gtk+-2.0 gtk+-3.0)

    target_include_directories(external_hime_im_client
    PUBLIC
        ${hime_im_client_src}
    PRIVATE
        ${hime_src}
        ${hime_src}/IMdkit/include
        ${GTK_INCLUDE_DIRS}
    )

    target_compile_definitions(external_hime_im_client
    PRIVATE
        HIME_BIN_DIR="/usr/bin"
        HIME_TABLE_DIR="/usr/share/hime/table"
        UNIX=1
        FREEBSD=0
        CLIENT_LIB=1
    )

    if (DESKTOP_APP_USE_PACKAGED)
        pkg_check_modules(X11 REQUIRED IMPORTED_TARGET x11)
        pkg_check_modules(GLIB2 REQUIRED IMPORTED_TARGET glib-2.0)

        target_link_libraries(external_hime_im_client
        PUBLIC
            PkgConfig::X11
            PkgConfig::GLIB2
        )
    else()
        pkg_check_modules(X11 REQUIRED x11)
        pkg_check_modules(GLIB2 REQUIRED glib-2.0)

        target_include_directories(external_hime_im_client
        PUBLIC
            ${X11_INCLUDE_DIRS}
            ${GLIB2_INCLUDE_DIRS}
        )

        target_link_libraries(external_hime_im_client
        PUBLIC
            X11
            glib-2.0
        )
    endif()
endif()
