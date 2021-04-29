# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_td INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_td ALIAS external_td)

set(td_loc ${libs_loc}/td)
set(td_build_loc ${td_loc}/out/$<IF:$<CONFIG:Debug>,Debug,Release>)
if (WIN32)
    set(td_lib_prefix $<CONFIG>/)
    set(td_lib_suffix .lib)
else()
    set(td_lib_prefix lib)
    set(td_lib_suffix .a)
endif()

target_include_directories(external_td
INTERFACE
    ${td_loc}
    ${td_loc}/td
    ${td_loc}/tdactor
    ${td_loc}/tdutils
    ${td_loc}/td/generate/auto
    ${td_build_loc}/tdutils
)

set(td_libs
    ${td_lib_prefix}tdclient
    ${td_lib_prefix}tdapi
    ${td_lib_prefix}tdmtproto
    ${td_lib_prefix}tdcore
    tdnet/${td_lib_prefix}tdnet
    tddb/${td_lib_prefix}tddb
    tdtl/${td_lib_prefix}tdtl
    tdactor/${td_lib_prefix}tdactor
    tdutils/${td_lib_prefix}tdutils
    sqlite/${td_lib_prefix}tdsqlite
)
foreach (lib ${td_libs})
    list(APPEND td_libs_list "${td_build_loc}/${lib}${td_lib_suffix}")
endforeach()

target_link_libraries(external_td
INTERFACE
    ${td_libs_list}
)

if (WIN32)
    target_link_libraries(external_td
    INTERFACE
        Psapi
        Normaliz
    )
endif()
