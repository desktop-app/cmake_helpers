# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

set(dispatch_loc ${third_party_loc}/dispatch)
set(dispatch_prefix ${CMAKE_CURRENT_BINARY_DIR}/dispatch-prefix)
set(dispatch_binary_dir ${dispatch_prefix}/src/dispatch-build)

if (DESKTOP_APP_USE_PACKAGED)
    find_library(DESKTOP_APP_DISPATCH_LIBRARIES dispatch)
    find_path(DESKTOP_APP_DISPATCH_INCLUDE_DIRS dispatch/dispatch.h)
endif()

if (NOT DESKTOP_APP_DISPATCH_LIBRARIES OR NOT DESKTOP_APP_DISPATCH_INCLUDE_DIRS)
    if (NOT DESKTOP_APP_USE_PACKAGED OR DESKTOP_APP_SPECIAL_TARGET)
        set(dispatch_extra_args
            -DCMAKE_BUILD_TYPE=Release
            "-DCMAKE_C_FLAGS=-g -pipe -fPIC -fstack-protector-all -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS"
            "-DCMAKE_CXX_FLAGS=-g -pipe -fPIC -fstack-protector-all -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS"
        )
    elseif (DEFINED CMAKE_BUILD_TYPE)
        set(dispatch_extra_args
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        )
    else()
        set(dispatch_extra_args)
    endif()
    execute_process(
        COMMAND ${CMAKE_COMMAND} ${dispatch_loc}
            -B ${dispatch_binary_dir}
            -GNinja
            -DCMAKE_C_COMPILER=clang
            -DCMAKE_CXX_COMPILER=clang++
            -DBUILD_SHARED_LIBS=OFF
            -DBUILD_TESTING=OFF
            ${dispatch_extra_args}
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${dispatch_binary_dir} --parallel
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

if (DESKTOP_APP_USE_PACKAGED AND DESKTOP_APP_DISPATCH_LIBRARIES AND DESKTOP_APP_DISPATCH_INCLUDE_DIRS)
    target_include_directories(external_dispatch SYSTEM INTERFACE ${DESKTOP_APP_DISPATCH_INCLUDE_DIRS})
    target_link_libraries(external_dispatch INTERFACE ${DESKTOP_APP_DISPATCH_LIBRARIES})
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
