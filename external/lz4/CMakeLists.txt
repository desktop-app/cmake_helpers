# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_lz4 INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_lz4 ALIAS external_lz4)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LZ4 REQUIRED liblz4)

    target_include_directories(external_lz4 INTERFACE ${LZ4_INCLUDE_DIRS})
    target_link_libraries(external_lz4 INTERFACE ${LZ4_LIBRARIES})
else()
    add_library(external_lz4 OBJECT)
    add_library(desktop-app::external_lz4 ALIAS external_lz4)
    init_target(external_lz4 "(external)")

    set(lz4_loc ${third_party_loc}/lz4/lib)

    target_sources(external_lz4
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

    target_include_directories(external_lz4
    PUBLIC
        ${lz4_loc}
    )
endif()
