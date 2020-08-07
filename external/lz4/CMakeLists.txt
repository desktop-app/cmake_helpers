# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_lz4 INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_lz4 ALIAS external_lz4)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LZ4 IMPORTED_TARGET liblz4)

    if (LZ4_FOUND)
        target_link_libraries(external_lz4 INTERFACE PkgConfig::LZ4)
    endif()
endif()

if (NOT LZ4_FOUND)
    add_library(external_lz4_bundled STATIC)
    init_target(external_lz4_bundled "(external)")

    set(lz4_loc ${third_party_loc}/lz4/lib)

    target_sources(external_lz4_bundled
    PRIVATE
        ${lz4_loc}/lz4.c
        ${lz4_loc}/lz4.h
        ${lz4_loc}/lz4frame.c
        ${lz4_loc}/lz4frame.h
        ${lz4_loc}/lz4frame_static.h
        ${lz4_loc}/lz4hc.c
        ${lz4_loc}/lz4hc.h
        ${lz4_loc}/xxhash.c
        ${lz4_loc}/xxhash.h
    )

    target_include_directories(external_lz4_bundled
    PUBLIC
        ${lz4_loc}
    )

    target_link_libraries(external_lz4
    INTERFACE
        external_lz4_bundled
    )
endif()
