# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_openssl_common INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_openssl_common ALIAS external_openssl_common)

if (NOT DESKTOP_APP_USE_PACKAGED)
    if (LINUX)
        target_include_directories(external_openssl_common SYSTEM
        INTERFACE
            /usr/local/desktop-app/openssl-1.1.1/include
        )
    else()
        target_include_directories(external_openssl_common SYSTEM
        INTERFACE
            ${libs_loc}/openssl_1_1_1/include
        )
    endif()

    if (LINUX)
        if (DESKTOP_APP_USE_GLIBC_WRAPS)
            target_link_libraries(external_openssl_common
            INTERFACE
                desktop-app::linux_glibc_wraps
                $<TARGET_FILE:desktop-app::linux_glibc_wraps>
            )
        endif()

        target_link_libraries(external_openssl_common
        INTERFACE
            ${CMAKE_DL_LIBS}
            pthread
        )
    endif()
endif()
