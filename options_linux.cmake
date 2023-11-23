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
    -Wno-stringop-overflow # Qt + LTO
    -Wno-odr # Qt + LTO
    -Wno-inline # OpenAL + LTO
    -pthread
    -Wl,--as-needed
)

if (DESKTOP_APP_SPECIAL_TARGET)
    target_compile_options(common_options
    INTERFACE
        -Werror
        $<$<NOT:$<CONFIG:Debug>>:-g>
        $<$<NOT:$<CONFIG:Debug>>:-flto=auto>
    )
    target_link_options(common_options
    INTERFACE
        $<$<NOT:$<CONFIG:Debug>>:-flto=auto>
        $<$<NOT:$<CONFIG:Debug>>:-fwhole-program>
    )
endif()

if (NOT DESKTOP_APP_USE_PACKAGED)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(common_options
        INTERFACE
            -static-libstdc++
            -static-libgcc
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
    set(interprocedural_optimization_config $<TARGET_PROPERTY:INTERPROCEDURAL_OPTIMIZATION_$<UPPER_CASE:$<CONFIG>>>)
    set(interprocedural_optimization_value_on -fwhole-program)
    set(interprocedural_optimization_value_off -fuse-ld=lld -fno-use-linker-plugin)
    set(interprocedural_optimization_values ${interprocedural_optimization_value_on},$<$<OR:$<NOT:$<BOOL:${DESKTOP_APP_SPECIAL_TARGET}>>,$<CONFIG:Debug>>:${interprocedural_optimization_value_off}>)
    target_link_options(common_options
    INTERFACE
        $<IF:$<NOT:$<STREQUAL:${interprocedural_optimization_config},>>,$<IF:$<BOOL:${interprocedural_optimization_config}>,${interprocedural_optimization_values}>,$<IF:$<BOOL:$<TARGET_PROPERTY:INTERPROCEDURAL_OPTIMIZATION>>,${interprocedural_optimization_values}>>
        -rdynamic
        -Wl,-z,muldefs
    )
endif()

if (NOT DESKTOP_APP_USE_PACKAGED OR DESKTOP_APP_SPECIAL_TARGET)
    target_compile_options_if_exists(common_options
    INTERFACE
        -fno-omit-frame-pointer
        -fstack-protector-all
        -fstack-clash-protection
        -fcf-protection
    )
    target_link_options(common_options
    INTERFACE
        -Wl,-z,relro
        -Wl,-z,now
        -Wl,-z,noexecstack
        -pie
    )
    target_compile_definitions(common_options
    INTERFACE
        $<$<NOT:$<CONFIG:Debug>>:_FORTIFY_SOURCE=3>
        _GLIBCXX_ASSERTIONS
    )
endif()

if (NOT DESKTOP_APP_DISABLE_SCUDO)
    target_link_libraries(common_options
    INTERFACE
        $<LINK_ONLY:desktop-app::external_scudo>
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

target_link_libraries(common_options
INTERFACE
    ${CMAKE_DL_LIBS}
)
