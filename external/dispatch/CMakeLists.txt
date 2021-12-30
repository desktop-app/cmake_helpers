# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

set(dispatch_loc ${third_party_loc}/dispatch)
set(dispatch_prefix ${CMAKE_CURRENT_BINARY_DIR}/dispatch-prefix)
set(dispatch_binary_dir ${dispatch_prefix}/src/dispatch-build)

if (DESKTOP_APP_USE_PACKAGED)
    find_library(DISPATCH_LIBRARIES dispatch)
    find_path(DISPATCH_INCLUDE_DIRS dispatch/dispatch.h)
endif()

if (NOT DISPATCH_LIBRARIES OR NOT DISPATCH_INCLUDE_DIRS)
    execute_process(
        COMMAND ${CMAKE_COMMAND} ${dispatch_loc}
            -B ${dispatch_binary_dir}
            -GNinja
            -DCMAKE_BUILD_TYPE=Release
            -DCMAKE_C_COMPILER=clang
            -DCMAKE_CXX_COMPILER=clang++
            -DBUILD_SHARED_LIBS=OFF
            -DBUILD_TESTING=OFF
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${dispatch_binary_dir}
    )

    if (NOT EXISTS ${dispatch_binary_dir}/src/libdispatch.a)
        if (DESKTOP_APP_SPECIAL_TARGET)
            message(FATAL_ERROR "Dispatch build failed")
        else()
            return()
        endif()
    endif()
endif()

add_library(external_dispatch INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_dispatch ALIAS external_dispatch)

if (DESKTOP_APP_USE_PACKAGED AND DISPATCH_LIBRARIES AND DISPATCH_INCLUDE_DIRS)
    target_include_directories(external_dispatch SYSTEM INTERFACE ${DISPATCH_INCLUDE_DIRS})
    target_link_libraries(external_dispatch INTERFACE ${DISPATCH_LIBRARIES})
    return()
endif()

add_library(external_dispatch_bundled STATIC IMPORTED)

set_target_properties(external_dispatch_bundled PROPERTIES
    IMPORTED_LOCATION "${dispatch_binary_dir}/src/libdispatch.a"
)

target_include_directories(external_dispatch_bundled SYSTEM
INTERFACE
    ${dispatch_loc}
)

target_link_libraries(external_dispatch_bundled
INTERFACE
    ${dispatch_binary_dir}/src/BlocksRuntime/libBlocksRuntime.a
)

target_link_libraries(external_dispatch
INTERFACE
    external_dispatch_bundled
)
