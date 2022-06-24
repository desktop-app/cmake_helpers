# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qoi STATIC)
init_target(external_qoi "(external)")
add_library(desktop-app::external_qoi ALIAS external_qoi)

set(qoi_loc ${third_party_loc}/qoi)

nice_target_sources(external_qoi ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    qoi_helper.cpp
)

target_compile_definitions(external_qoi
PUBLIC
    QOI_NO_STDIO
PRIVATE
    QOI_IMPLEMENTATION
)

target_include_directories(external_qoi
PUBLIC
    ${qoi_loc}
)
