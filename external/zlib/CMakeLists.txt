# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_zlib INTERFACE IMPORTED GLOBAL)

target_include_directories(external_zlib SYSTEM
INTERFACE
    ${libs_loc}/zlib
    ${libs_loc}/zlib/contrib/minizip
)

if (WIN32)
    target_compile_definitions(external_zlib
    INTERFACE
        ZLIB_WINAPI
    )
endif()

target_link_directories(external_zlib
INTERFACE
    ${libs_loc}/zlib/contrib/vstudio/vc14/x86/ZlibStat$<IF:$<CONFIG:Debug>,Debug,ReleaseWithoutAsm>
)

target_link_libraries(external_zlib
INTERFACE
    zlibstat
)
