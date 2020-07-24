# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND NOT DESKTOP_APP_USE_PACKAGED_LAZY_PLATFORMTHEMES)
    add_library(external_xdgiconloader INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_xdgiconloader ALIAS external_xdgiconloader)
else()
    add_library(external_xdgiconloader STATIC)
    add_library(desktop-app::external_xdgiconloader ALIAS external_xdgiconloader)
    init_target(external_xdgiconloader "(external)")

    set(libqtxdg_loc ${third_party_loc}/libqtxdg)
    set(xdgiconloader_src ${libqtxdg_loc}/src/xdgiconloader)
    set(xdgiconloader_includedir "${CMAKE_CURRENT_BINARY_DIR}/xdgiconloader")

    set_target_properties(external_xdgiconloader PROPERTIES AUTOMOC ON)

    nice_target_sources(external_xdgiconloader ${xdgiconloader_src}
    PRIVATE
        xdgiconloader.cpp
        xdgiconloader_p.h
    )

    include(GenerateExportHeader)
    generate_export_header(external_xdgiconloader BASE_NAME XdgIconLoader)

    configure_file(
        "${CMAKE_CURRENT_BINARY_DIR}/xdgiconloader_export.h"
        "${xdgiconloader_includedir}/xdgiconloader_export.h"
        COPYONLY
    )

    file(
    COPY
        ${xdgiconloader_src}/xdgiconloader_p.h
    DESTINATION
        ${xdgiconloader_includedir}/private/xdgiconloader
    )

    target_include_directories(external_xdgiconloader
    PUBLIC
        ${xdgiconloader_includedir}
    PRIVATE
        ${xdgiconloader_src}
    )

    target_compile_definitions(external_xdgiconloader
    PRIVATE
        QT_NO_KEYWORDS
    )

    target_link_libraries(external_xdgiconloader
    PRIVATE
        desktop-app::external_qt
    )
endif()
