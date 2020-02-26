# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_ton INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_ton ALIAS external_ton)

set(ton_loc ${libs_loc}/ton)
set(ton_build_loc ${ton_loc}/build$<$<CONFIG:Debug>:-debug>)
if (WIN32)
    set(ton_libs_mid /$<CONFIG>)
else()
    set(ton_libs_mid "")
endif()

target_include_directories(external_ton
INTERFACE
    ${ton_loc}
    ${ton_loc}/crypto
    ${ton_loc}/tdactor
    ${ton_loc}/tdutils
    ${ton_loc}/tonlib
    ${ton_loc}/tl
    ${ton_loc}/tl/generate
    ${ton_build_loc}/tdutils
)

if (WIN32)
    set(ton_lib_suffix .lib)
else()
    set(ton_lib_suffix .a)
endif()

set(ton_libs
    tonlib${ton_libs_mid}/tonlib
    tl${ton_libs_mid}/tl_tonlib_api
    crypto${ton_libs_mid}/smc-envelope
    lite-client${ton_libs_mid}/lite-client-common
    crypto${ton_libs_mid}/ton_block
    adnl${ton_libs_mid}/adnllite
    tl-utils${ton_libs_mid}/tl-lite-utils
    keys${ton_libs_mid}/keys
    tl-utils${ton_libs_mid}/tl-utils
    tl${ton_libs_mid}/tl_api
    tl${ton_libs_mid}/tl_lite_api
    tdnet${ton_libs_mid}/tdnet
    tdactor${ton_libs_mid}/tdactor
    crypto${ton_libs_mid}/ton_crypto
    tdutils${ton_libs_mid}/tdutils
    third-party/crc32c${ton_libs_mid}/crc32c
)
foreach (lib ${ton_libs})
    list(APPEND ton_libs_list "${ton_build_loc}/${lib}${ton_lib_suffix}")
endforeach()

target_link_libraries(external_ton
INTERFACE
    ${ton_libs_list}
)
