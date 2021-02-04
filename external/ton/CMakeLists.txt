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
    set(ton_lib_prefix $<CONFIG>/)
    set(ton_lib_suffix .lib)
else()
    set(ton_lib_prefix lib)
    set(ton_lib_suffix .a)
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

set(ton_libs
    tonlib/${ton_lib_prefix}tonlib
    tl/${ton_lib_prefix}tl_tonlib_api
    crypto/${ton_lib_prefix}smc-envelope
    lite-client/${ton_lib_prefix}lite-client-common
    crypto/${ton_lib_prefix}ton_block
    adnl/${ton_lib_prefix}adnllite
    tl-utils/${ton_lib_prefix}tl-lite-utils
    keys/${ton_lib_prefix}keys
    tl-utils/${ton_lib_prefix}tl-utils
    tl/${ton_lib_prefix}tl_api
    tl/${ton_lib_prefix}tl_lite_api
    tdnet/${ton_lib_prefix}tdnet
    tdactor/${ton_lib_prefix}tdactor
    crypto/${ton_lib_prefix}ton_crypto
    tdutils/${ton_lib_prefix}tdutils
    third-party/crc32c/${ton_lib_prefix}crc32c
)
foreach (lib ${ton_libs})
    list(APPEND ton_libs_list "${ton_build_loc}/${lib}${ton_lib_suffix}")
endforeach()

target_link_libraries(external_ton
INTERFACE
    ${ton_libs_list}
)

if (WIN32)
    target_link_libraries(external_ton
    INTERFACE
        Psapi
        Normaliz
    )
endif()
