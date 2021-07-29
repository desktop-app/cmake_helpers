# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

target_compile_options(common_options
INTERFACE
    -fPIC
    $<IF:$<CONFIG:Debug>,,-fno-strict-aliasing>
    -pipe
    -Wall
    -W
    -Wno-unused-parameter
    -Wno-switch
    -Wno-missing-field-initializers
    -Wno-sign-compare
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(common_options
    INTERFACE
        -Wno-maybe-uninitialized
    )
endif()

if (DESKTOP_APP_SPECIAL_TARGET)
    target_compile_options(common_options
    INTERFACE
        $<IF:$<CONFIG:Debug>,,-Ofast>
        -Werror
    )

    target_link_options(common_options
    INTERFACE
        $<IF:$<CONFIG:Debug>,,-Ofast>
    )

    if (build_linux32)
        target_compile_options(common_options INTERFACE -g0)
        target_link_options(common_options INTERFACE -g0)
    else()
        target_compile_options(common_options INTERFACE $<IF:$<CONFIG:Debug>,,-g -flto>)
        target_link_options(common_options INTERFACE $<IF:$<CONFIG:Debug>,,-g -flto -fuse-linker-plugin>)
    endif()
endif()

target_link_libraries(common_options
INTERFACE
    desktop-app::external_jemalloc
)

if (DESKTOP_APP_USE_ALLOCATION_TRACER)
    target_link_options(common_options
    INTERFACE
        # -Wl,-wrap,__malloc
        -Wl,-wrap,__libc_malloc
        -Wl,-wrap,malloc
        -Wl,-wrap,valloc
        -Wl,-wrap,pvalloc
        -Wl,-wrap,calloc
        -Wl,-wrap,realloc
        -Wl,-wrap,memalign
        -Wl,-wrap,aligned_alloc
        -Wl,-wrap,posix_memalign
        -Wl,-wrap,free
        -Wl,--no-as-needed,-lrt
    )
    target_link_libraries(common_options
    INTERFACE
        desktop-app::linux_allocation_tracer
        $<TARGET_FILE:desktop-app::linux_allocation_tracer>
    )
endif()

if (NOT DESKTOP_APP_USE_PACKAGED)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(common_options
        INTERFACE
            -static-libstdc++
        )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_link_static_libraries(common_options
        INTERFACE
            c++
            c++abi
        )
        target_link_options(common_options
        INTERFACE
            -nostdlib++
        )
    endif()
    target_link_options(common_options
    INTERFACE
        -pthread
        -rdynamic
    )
endif()

if (DESKTOP_APP_USE_PACKAGED)
    find_library(ATOMIC_LIBRARY atomic)
else()
    find_library(ATOMIC_LIBRARY libatomic.a)
endif()

if (ATOMIC_LIBRARY)
    target_link_libraries(common_options
    INTERFACE
        ${ATOMIC_LIBRARY}
    )
endif()
