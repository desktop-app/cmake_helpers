# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_lzma INTERFACE IMPORTED GLOBAL)

target_include_directories(external_lzma SYSTEM
INTERFACE
    ${libs_loc}/lzma/C
)

target_link_directories(external_lzma
INTERFACE
    ${libs_loc}/lzma/C/Util/LzmaLib/$<IF:$<CONFIG:Debug>,Debug,Release>
)

target_link_libraries(external_lzma
INTERFACE
    LzmaLib
)
