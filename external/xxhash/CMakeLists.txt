# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_xxhash INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_xxhash ALIAS external_xxhash)

if (DESKTOP_APP_USE_PACKAGED)
    find_library(XXHASH_LIBRARY xxhash)
    find_path(XXHASH_INCLUDE_DIRS xxhash.h)

    if (XXHASH_LIBRARY AND XXHASH_INCLUDE_DIRS)
        target_include_directories(external_xxhash INTERFACE ${XXHASH_INCLUDE_DIRS})
        target_link_libraries(external_xxhash INTERFACE ${XXHASH_LIBRARY})
    endif()
endif()

if (NOT XXHASH_LIBRARY OR NOT XXHASH_INCLUDE_DIRS)
    target_include_directories(external_xxhash SYSTEM
    INTERFACE
        ${third_party_loc}/xxHash
    )

    target_compile_definitions(external_xxhash
    INTERFACE
        XXH_INLINE_ALL
    )
endif()
