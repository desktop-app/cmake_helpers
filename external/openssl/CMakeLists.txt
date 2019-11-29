# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_openssl INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_openssl ALIAS external_openssl)

target_include_directories(external_openssl SYSTEM
INTERFACE
    ${libs_loc}/openssl_1_1_1/include
)

if (WIN32)
    set(openssl_lib_ext lib)
    set(openssl_lib_loc ${libs_loc}/openssl_1_1_1/out32$<$<CONFIG:Debug>:.dbg>)
else()
    set(openssl_lib_ext a)
    if (APPLE)
        set(openssl_lib_loc ${libs_loc}/openssl_1_1_1)
    endif()
endif()

target_link_libraries(external_openssl
INTERFACE
    ${openssl_lib_loc}/libcrypto.${openssl_lib_ext}
    ${openssl_lib_loc}/libssl.${openssl_lib_ext}
)
