# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_mallocng STATIC)
add_library(desktop-app::external_mallocng ALIAS external_mallocng)
init_target(external_mallocng "(external)")

set(mallocng_loc ${third_party_loc}/mallocng)

nice_target_sources(external_mallocng ${mallocng_loc}
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
)

target_compile_options(external_mallocng
PRIVATE
    -Wno-unused-value
)

target_include_directories(external_mallocng
PRIVATE
    ${mallocng_loc}
)

target_compile_options(external_mallocng
PUBLIC
    -fno-builtin-malloc
    -fno-builtin-calloc
    -fno-builtin-realloc
    -fno-builtin-free
)
