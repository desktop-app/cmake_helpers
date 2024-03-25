# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_openssl_crypto INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_openssl_crypto ALIAS external_openssl_crypto)

    find_package(OpenSSL REQUIRED)
    target_link_libraries(external_openssl_crypto INTERFACE OpenSSL::Crypto)
    return()
endif()

add_library(external_openssl_crypto STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_openssl_crypto ALIAS external_openssl_crypto)

if (WIN32)
    set_target_properties(external_openssl_crypto PROPERTIES
        IMPORTED_LOCATION "${openssl_lib_loc}/libcrypto${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${openssl_lib_loc}.dbg/libcrypto${CMAKE_STATIC_LIBRARY_SUFFIX}"
    )
elseif (APPLE)
    set_target_properties(external_openssl_crypto PROPERTIES
        IMPORTED_LOCATION "${openssl_lib_loc}/libcrypto${CMAKE_STATIC_LIBRARY_SUFFIX}"
    )
else()
    find_library(DESKTOP_APP_CRYPTO_LIBRARIES libcrypto.a REQUIRED)
    set_target_properties(external_openssl_crypto PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_CRYPTO_LIBRARIES}"
    )
endif()

target_link_libraries(external_openssl_crypto
INTERFACE
    desktop-app::external_openssl_common
)
