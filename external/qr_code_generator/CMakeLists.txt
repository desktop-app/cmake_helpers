# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

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
