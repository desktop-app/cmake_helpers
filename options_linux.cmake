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
        target_compile_options(common_options INTERFACE $<IF:$<CONFIG:Debug>,,-g>)
        target_link_options(common_options INTERFACE $<IF:$<CONFIG:Debug>,,-g>)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(common_options INTERFACE $<IF:$<CONFIG:Debug>,,-flto>)
            target_link_options(common_options INTERFACE $<IF:$<CONFIG:Debug>,,-flto -fuse-linker-plugin>)
        endif()
    endif()
endif()

target_link_libraries(common_options
INTERFACE
    desktop-app::external_jemalloc
)

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
