# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_openssl INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_openssl ALIAS external_openssl)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(external_openssl INTERFACE OpenSSL::SSL)
else()
    if (LINUX)
        target_include_directories(external_openssl SYSTEM
        INTERFACE
            /usr/local/desktop-app/openssl-1.1.1/include
        )
    elseif (NOT APPLE OR NOT build_osx)
        target_include_directories(external_openssl SYSTEM
        INTERFACE
            ${libs_loc}/openssl_1_1_1/include
        )
    else()
        target_include_directories(external_openssl SYSTEM
        INTERFACE
            ${libs_loc}/openssl/include
        )
    endif()

    if (WIN32)
        set(openssl_lib_ext lib)
        set(openssl_lib_loc ${libs_loc}/openssl_1_1_1/out32$<$<CONFIG:Debug>:.dbg>)
    else()
        set(openssl_lib_ext a)
        if (APPLE)
            if (NOT build_osx)
                set(openssl_lib_loc ${libs_loc}/openssl_1_1_1)
            else()
                set(openssl_lib_loc ${libs_loc}/openssl)
            endif()
        else()
            set(openssl_lib_loc /usr/local/desktop-app/openssl-1.1.1/lib)
        endif()
    endif()

    target_link_libraries(external_openssl
    INTERFACE
        ${openssl_lib_loc}/libssl.${openssl_lib_ext}
        ${openssl_lib_loc}/libcrypto.${openssl_lib_ext}
    )

    if (LINUX)
        if (DESKTOP_APP_USE_GLIBC_WRAPS)
            target_link_libraries(external_openssl
            INTERFACE
                desktop-app::linux_glibc_wraps
                $<TARGET_FILE:desktop-app::linux_glibc_wraps>
            )
        endif()
        target_link_libraries(external_openssl INTERFACE pthread)
    endif()
endif()
