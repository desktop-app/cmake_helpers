# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qr_code_generator INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_qr_code_generator ALIAS external_qr_code_generator)

if (DESKTOP_APP_USE_PACKAGED)
    find_library(QRCODE_LIBRARIES NAMES qrcodegencpp)
    find_path(QRCODE_INCLUDE_DIRS NAMES qrcodegen.hpp QrCode.hpp PATH_SUFFIXES qrcodegencpp qrcodegen)

    if (QRCODE_LIBRARIES AND QRCODE_INCLUDE_DIRS)
        target_include_directories(external_qr_code_generator SYSTEM INTERFACE ${QRCODE_INCLUDE_DIRS})
        target_link_libraries(external_qr_code_generator INTERFACE ${QRCODE_LIBRARIES})
        return()
    endif()
endif()

add_library(external_qr_code_generator_bundled STATIC)
init_target(external_qr_code_generator_bundled "(external)")

set(qr_loc ${third_party_loc}/QR)
set(qr_src ${qr_loc}/cpp)

nice_target_sources(external_qr_code_generator_bundled ${qr_src}
PRIVATE
    qrcodegen.cpp
    qrcodegen.hpp
)

target_include_directories(external_qr_code_generator_bundled
PUBLIC
    ${qr_src}
)

target_link_libraries(external_qr_code_generator
INTERFACE
    external_qr_code_generator_bundled
)
