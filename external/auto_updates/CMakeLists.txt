# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_auto_updates INTERFACE IMPORTED GLOBAL)

if (WIN32)
    add_subdirectory(lzma)
    target_link_libraries(external_auto_updates
    INTERFACE
        external_lzma
    )
else()
    add_subdirectory(xz)
    target_link_libraries(external_auto_updates
    INTERFACE
        external_xz
    )
endif()
