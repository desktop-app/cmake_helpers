# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_scudo STATIC)
add_library(desktop-app::external_scudo ALIAS external_scudo)
init_target(external_scudo "(external)")

set(scudo_loc ${third_party_loc}/scudo)

# https://llvm.org/docs/ScudoHardenedAllocator.html#library
file(GLOB scudo_srcs ${scudo_loc}/*.cpp)
target_sources(external_scudo
PRIVATE
    ${scudo_srcs}
)

target_include_directories(external_scudo SYSTEM
PRIVATE
    ${scudo_loc}/include
)

add_library(external_scudo_options INTERFACE)
target_compile_options(external_scudo_options
INTERFACE
    -fno-lto
    -Wno-unused-result
)

target_link_libraries(external_scudo
PRIVATE
    external_scudo_options
)
