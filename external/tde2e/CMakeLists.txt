# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_tde2e INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_tde2e ALIAS external_tde2e)

if (DESKTOP_APP_USE_PACKAGED OR LINUX)
    find_package(tde2e REQUIRED)
    target_link_libraries(external_tde2e INTERFACE tde2e::tde2e)
    return()
endif()

set(tde2e_loc ${libs_loc}/tde2e)
set(tde2e_build_loc ${tde2e_loc}/out/$<IF:$<CONFIG:Debug>,Debug,Release>)
if (WIN32)
    set(tde2e_lib_prefix $<IF:$<CONFIG:Debug>,Debug,Release>/)
    set(tde2e_lib_suffix .lib)
else()
    set(tde2e_lib_prefix lib)
    set(tde2e_lib_suffix .a)
endif()

target_include_directories(external_tde2e
INTERFACE
    ${tde2e_loc}/tde2e
)

set(tde2e_libs
    tde2e/${tde2e_lib_prefix}tde2e
    tdutils/${tde2e_lib_prefix}tdutils
)
foreach (lib ${tde2e_libs})
    list(APPEND tde2e_libs_list "${tde2e_build_loc}/${lib}${tde2e_lib_suffix}")
endforeach()

target_link_libraries(external_tde2e
INTERFACE
    ${tde2e_libs_list}
)

if (WIN32)
    target_link_libraries(external_tde2e
    INTERFACE
        Psapi
        Normaliz
    )
endif()
