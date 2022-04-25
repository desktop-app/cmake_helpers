# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

target_compile_options(common_options
INTERFACE
    -fstack-protector-all
    -fPIC
    $<IF:$<CONFIG:Debug>,,-fno-strict-aliasing>
    -pipe
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-switch
    -Wno-missing-field-initializers
    -Wno-sign-compare
    -Wno-deprecated # implicit capture of 'this' via '[=]' is deprecated in C++20
)

target_compile_definitions(common_options
INTERFACE
    $<IF:$<CONFIG:Debug>,,_FORTIFY_SOURCE=2>
    _GLIBCXX_ASSERTIONS
)

target_link_options(common_options
INTERFACE
    -Wl,--as-needed
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(common_options
    INTERFACE
        -fstack-clash-protection
        -Wno-maybe-uninitialized
    )
endif()

# TODO: Remove when there will be no Qt 5 support
if (DESKTOP_APP_QT6)
    target_compile_options(common_options
    INTERFACE
        -Wno-deprecated-declarations
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

    target_compile_options(common_options INTERFACE $<IF:$<CONFIG:Debug>,,-g -flto>)
    target_link_options(common_options INTERFACE $<IF:$<CONFIG:Debug>,,-g -flto -fuse-linker-plugin>)
endif()

if (NOT DESKTOP_APP_DISABLE_JEMALLOC)
	target_link_libraries(common_options
	INTERFACE
	    $<TARGET_OBJECTS:desktop-app::linux_jemalloc_helper>
	    $<LINK_ONLY:desktop-app::external_jemalloc>
	)
endif()

target_link_libraries(common_options
INTERFACE
    ${CMAKE_DL_LIBS}
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
        -Wl,--push-state,--no-as-needed,-lrt,--pop-state
    )
    target_link_libraries(common_options
    INTERFACE
        desktop-app::linux_allocation_tracer
        $<TARGET_FILE:desktop-app::linux_allocation_tracer>
    )
endif()

if (DESKTOP_APP_USE_PACKAGED)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(common_options
    INTERFACE
        Threads::Threads
    )
else()
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
    target_link_options(common_options
    INTERFACE
        -pthread
        -rdynamic
        -fwhole-program
        -Wl,-z,relro
        -Wl,-z,now
        # -pie # https://gitlab.gnome.org/GNOME/nautilus/-/issues/1601
    )
endif()
