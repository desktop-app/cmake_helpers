# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

target_compile_options(common_options
INTERFACE
    $<IF:$<CONFIG:Debug>,,-Ofast -fno-strict-aliasing>
    -pipe
    -Wall
    -Werror
    -W
    -fPIC
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

