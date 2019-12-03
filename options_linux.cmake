# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

target_compile_options(common_options
INTERFACE
    -g
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
    -Wno-error=class-memaccess
)
target_link_options(common_options
INTERFACE
    -g
)
