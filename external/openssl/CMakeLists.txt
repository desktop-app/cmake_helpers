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

target_link_directories(external_openssl
INTERFACE
    ${libs_loc}/openssl_1_1_1/out32$<$<CONFIG:Debug>:.dbg>
)

target_link_libraries(external_openssl
INTERFACE
    libcrypto
    libssl
)
