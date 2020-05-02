# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED_QRCODE AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_qr_code_generator INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_qr_code_generator ALIAS external_qr_code_generator)

    find_library(QRCODE_LIBRARIES NAMES qrcodegencpp)
    find_path(QRCODE_INCLUDE_DIRS QrCode.hpp PATH_SUFFIXES qrcodegencpp qrcodegen)

    target_include_directories(external_qr_code_generator INTERFACE ${QRCODE_INCLUDE_DIRS})
    target_link_libraries(external_qr_code_generator INTERFACE ${QRCODE_LIBRARIES})
else()
    add_library(external_qr_code_generator OBJECT)
    add_library(desktop-app::external_qr_code_generator ALIAS external_qr_code_generator)
    init_target(external_qr_code_generator "(external)")

    set(qr_loc ${third_party_loc}/QR)
    set(qr_src ${qr_loc}/cpp)

    nice_target_sources(external_qr_code_generator ${qr_src}
    PRIVATE
        BitBuffer.cpp
        BitBuffer.hpp
        QrCode.cpp
        QrCode.hpp
        QrSegment.cpp
        QrSegment.hpp
    )

    target_include_directories(external_qr_code_generator
    PUBLIC
        ${qr_src}
    )
endif()
