# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_mallocng INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_mallocng ALIAS external_mallocng)

add_library(external_mallocng_bundled STATIC)
init_target(external_mallocng_bundled "(external)")

set(mallocng_loc ${third_party_loc}/mallocng)

nice_target_sources(external_mallocng_bundled ${mallocng_loc}
PRIVATE
    malloc.c
    calloc.c
    free.c
    realloc.c
    aligned_alloc.c
    posix_memalign.c
    memalign.c
    malloc_usable_size.c
    meta.h
    glue.h
    valloc.c
    pvalloc.c
)

target_compile_options(external_mallocng_bundled
PRIVATE
    -Wno-unused-value
)

target_include_directories(external_mallocng_bundled
PRIVATE
    ${mallocng_loc}
)

target_link_libraries(external_mallocng
INTERFACE
    -Wl,--whole-archive
    external_mallocng_bundled
    -Wl,--no-whole-archive
)
