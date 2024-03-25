# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED)
    add_library(external_openssl_ssl INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_openssl_ssl ALIAS external_openssl_ssl)

    find_package(OpenSSL REQUIRED)
    target_link_libraries(external_openssl_ssl INTERFACE OpenSSL::SSL)
    return()
endif()

add_library(external_openssl_ssl STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_openssl_ssl ALIAS external_openssl_ssl)

if (WIN32)
    set_target_properties(external_openssl_ssl PROPERTIES
        IMPORTED_LOCATION "${openssl_lib_loc}/libssl${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${openssl_lib_loc}.dbg/libssl${CMAKE_STATIC_LIBRARY_SUFFIX}"
    )
elseif (APPLE)
    set_target_properties(external_openssl_ssl PROPERTIES
        IMPORTED_LOCATION "${openssl_lib_loc}/libssl${CMAKE_STATIC_LIBRARY_SUFFIX}"
    )
else()
    find_library(DESKTOP_APP_SSL_LIBRARIES libssl.a REQUIRED)
    set_target_properties(external_openssl_ssl PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_SSL_LIBRARIES}"
    )
endif()

target_link_libraries(external_openssl_ssl
INTERFACE
    desktop-app::external_openssl_common
)
