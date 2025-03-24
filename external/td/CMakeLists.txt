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
    ${td_loc}/tde2e
    ${td_loc}/tdutils
    ${td_build_loc}/tdutils
)

set(td_libs
    tde2e/${td_lib_prefix}tde2e
    tdutils/${td_lib_prefix}tdutils
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
