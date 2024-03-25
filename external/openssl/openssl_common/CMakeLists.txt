# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_openssl_common INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_openssl_common ALIAS external_openssl_common)

if (NOT DESKTOP_APP_USE_PACKAGED)
    if (NOT LINUX)
        target_include_directories(external_openssl_common SYSTEM
        INTERFACE
            ${libs_loc}/openssl3/include
        )
    endif()

    # OpenSSL 3 deprecated AES_set_encrypt_key/decrypt_key/ige_encrypt/cbc_encrypt
    target_compile_options_if_exists(external_openssl_common
    INTERFACE
        -Wno-deprecated-declarations
    )
endif()
