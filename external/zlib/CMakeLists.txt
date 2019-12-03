# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_zlib INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_zlib ALIAS external_zlib)

if (NOT WIN32)
    add_library(external_minizip STATIC)
    init_target(external_minizip "(external)")

    set(minizip_loc ${third_party_loc}/minizip)

    target_sources(external_minizip
    PRIVATE
        ${minizip_loc}/crypt.h
        ${minizip_loc}/ioapi.c
        ${minizip_loc}/ioapi.h
        ${minizip_loc}/zip.c
        ${minizip_loc}/zip.h
        ${minizip_loc}/unzip.c
        ${minizip_loc}/unzip.h
    )

    target_include_directories(external_minizip SYSTEM
    INTERFACE
        ${minizip_loc}
    )

    target_link_libraries(external_zlib INTERFACE external_minizip)
endif()

target_include_directories(external_zlib SYSTEM
INTERFACE
    ${libs_loc}/zlib
    ${libs_loc}/zlib/contrib/minizip
)

if (WIN32)
    target_compile_definitions(external_zlib INTERFACE ZLIB_WINAPI)

    set(zlib_lib_loc ${libs_loc}/zlib/contrib/vstudio/vc14/x86/ZlibStat$<IF:$<CONFIG:Debug>,Debug,ReleaseWithoutAsm>)

    target_link_libraries(external_zlib INTERFACE ${zlib_lib_loc}/zlibstat.lib)
elseif (APPLE)
    target_link_libraries(external_zlib INTERFACE /usr/local/lib/libz.a)
else()
    target_link_static_libraries(external_zlib INTERFACE z)
endif()
