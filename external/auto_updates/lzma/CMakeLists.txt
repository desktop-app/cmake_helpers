# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_lzma INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_lzma ALIAS external_lzma)

target_include_directories(external_lzma SYSTEM
INTERFACE
    ${libs_loc}/lzma/C
)

if (build_win64)
    set(lzma_platform_dir x64/)
else()
    set(lzma_platform_dir "")
endif()

set(lzma_lib_loc ${libs_loc}/lzma/C/Util/LzmaLib/${lzma_platform_dir}$<IF:$<CONFIG:Debug>,Debug,Release>)

target_link_libraries(external_lzma
INTERFACE
    ${lzma_lib_loc}/LzmaLib.lib
)
