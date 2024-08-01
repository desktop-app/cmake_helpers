# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_ada INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_ada ALIAS external_ada)

    find_package(ada REQUIRED)
    target_link_libraries(external_ada INTERFACE ada::ada)
    return()
endif()

add_library(external_ada STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_ada ALIAS external_ada)

if (WIN32)
    set(ada_lib_loc ${libs_loc}/ada/out/singleheader)
    target_include_directories(external_ada SYSTEM
    INTERFACE
        ${ada_lib_loc}
    )
    set_target_properties(external_ada PROPERTIES
        IMPORTED_LOCATION "${ada_lib_loc}/Release/ada-singleheader-lib.lib"
        IMPORTED_LOCATION_DEBUG "${ada_lib_loc}/Debug/ada-singleheader-lib.lib"
    )
elseif (APPLE)
    target_include_directories(external_ada SYSTEM
    INTERFACE
        ${libs_loc}/local/include
    )
    set_target_properties(external_ada PROPERTIES
        IMPORTED_LOCATION ${libs_loc}/local/lib/libada.a
    )
else()
    target_include_directories(external_ada SYSTEM
    INTERFACE
        /usr/local/include
    )
    find_library(DESKTOP_APP_ADA_LIBRARIES libada.a REQUIRED)
    set_target_properties(external_ada PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_ADA_LIBRARIES}"
    )
endif()
