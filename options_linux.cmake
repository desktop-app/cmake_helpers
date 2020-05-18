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
    -Wno-unused-variable
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-switch
    -Wno-comment
    -Wno-unused-but-set-variable
    -Wno-missing-field-initializers
    -Wno-sign-compare
    -Wno-attributes
    -Wno-parentheses
    -Wno-stringop-overflow
    -Wno-maybe-uninitialized
    -Wno-error=class-memaccess
    $<$<NOT:$<COMPILE_LANGUAGE:C>>:-Wno-register>
)

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
