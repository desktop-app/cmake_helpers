# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

target_compile_options(common_options
INTERFACE
    -fPIC
    $<$<NOT:$<CONFIG:Debug>>:-fno-strict-aliasing>
    -pipe
)

target_compile_options_if_exists(common_options
INTERFACE
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-switch
    -Wno-maybe-uninitialized
    -Wno-missing-field-initializers
    -Wno-sign-compare
    -Wno-deprecated # implicit capture of 'this' via '[=]' is deprecated in C++20
)

target_link_options_if_exists(common_options
INTERFACE
    -Wno-alloc-size-larger-than # Qt + LTO
    -Wno-free-nonheap-object # Qt + LTO
    -Wno-stringop-overflow # Qt + LTO
    -Wno-stringop-overread # Qt + LTO
    -Wno-odr # Qt + LTO
    -Wno-inline # OpenAL + LTO
    -pthread
    -Wl,--as-needed
)

if (DESKTOP_APP_SPECIAL_TARGET)
    target_compile_options(common_options
    INTERFACE
        -Werror
    )
endif()

if (NOT DESKTOP_APP_USE_PACKAGED)
    set(ipo_prop $<TARGET_PROPERTY:INTERPROCEDURAL_OPTIMIZATION>)
    set(ipo_config_prop $<TARGET_PROPERTY:INTERPROCEDURAL_OPTIMIZATION_$<UPPER_CASE:$<CONFIG>>>)
    set(ipo_compile_value_on)
    set(ipo_compile_value_off -fno-lto)
    set(ipo_compile_values ${ipo_compile_value_on},${ipo_compile_value_off})
    set(ipo_link_value_on -fwhole-program)
    set(ipo_link_value_off -fuse-ld=lld -fno-lto -fno-use-linker-plugin)
    set(ipo_link_values ${ipo_link_value_on},${ipo_link_value_off})
    target_compile_options(common_options
    INTERFACE
        $<IF:$<NOT:$<STREQUAL:${ipo_config_prop},>>,$<IF:$<BOOL:${ipo_config_prop}>,${ipo_compile_values}>,$<IF:$<BOOL:${ipo_prop}>,${ipo_compile_values}>>
        $<$<CONFIG:Debug>:-O0>
    )
    target_link_options(common_options
    INTERFACE
        $<IF:$<NOT:$<STREQUAL:${ipo_config_prop},>>,$<IF:$<BOOL:${ipo_config_prop}>,${ipo_link_values}>,$<IF:$<BOOL:${ipo_prop}>,${ipo_link_values}>>
        -static-libstdc++
        -static-libgcc
        -rdynamic
        -Wl,-z,muldefs
    )
endif()

if (NOT DESKTOP_APP_DISABLE_JEMALLOC)
	target_link_libraries(common_options
	INTERFACE
	    $<TARGET_OBJECTS:desktop-app::linux_jemalloc_helper>
	    $<LINK_ONLY:desktop-app::external_jemalloc>
	)
endif()

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
        -Wl,--push-state,--no-as-needed,-lrt,--pop-state
    )
    target_link_libraries(common_options
    INTERFACE
        desktop-app::linux_allocation_tracer
        $<TARGET_FILE:desktop-app::linux_allocation_tracer>
    )
endif()

if (DESKTOP_APP_ASAN)
    target_compile_options(common_options INTERFACE -fsanitize=address)
    target_link_options(common_options INTERFACE -fsanitize=address)

    if (NOT DESKTOP_APP_USE_PACKAGED)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_link_options(common_options INTERFACE -static-libasan)
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            target_link_options(common_options INTERFACE -static-libsan)
        endif()
    endif()
endif()

target_link_libraries(common_options
INTERFACE
    ${CMAKE_DL_LIBS}
)
