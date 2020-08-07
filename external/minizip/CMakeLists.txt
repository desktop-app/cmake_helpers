# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_minizip INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_minizip ALIAS external_minizip)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(MINIZIP IMPORTED_TARGET minizip)

    if (MINIZIP_FOUND)
        target_link_libraries(external_minizip INTERFACE PkgConfig::MINIZIP)
    endif()
endif()

if (NOT MINIZIP_FOUND)
    if (NOT WIN32)
        add_library(external_minizip_bundled STATIC)
        init_target(external_minizip_bundled "(external)")

        set(minizip_loc ${third_party_loc}/minizip)

        target_sources(external_minizip_bundled
        PRIVATE
            ${minizip_loc}/crypt.h
            ${minizip_loc}/ioapi.c
            ${minizip_loc}/ioapi.h
            ${minizip_loc}/zip.c
            ${minizip_loc}/zip.h
            ${minizip_loc}/unzip.c
            ${minizip_loc}/unzip.h
        )

        target_include_directories(external_minizip_bundled SYSTEM
        PUBLIC
            ${minizip_loc}
        )

        target_link_libraries(external_minizip
        INTERFACE
            external_minizip_bundled
        )
    else()
        target_include_directories(external_minizip SYSTEM
        INTERFACE
            ${libs_loc}/zlib/contrib/minizip
        )
    endif()
endif()
