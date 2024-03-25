# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_openssl INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_openssl ALIAS external_openssl)

if (WIN32)
    set(openssl_lib_loc ${libs_loc}/openssl3/out)
elseif (APPLE)
    set(openssl_lib_loc ${libs_loc}/openssl3)
endif()

add_subdirectory(openssl_common)
add_subdirectory(openssl_ssl)
add_subdirectory(openssl_crypto)

target_link_libraries(external_openssl
INTERFACE
    desktop-app::external_openssl_ssl
    desktop-app::external_openssl_crypto
)
