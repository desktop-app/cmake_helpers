# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(linux_glibc_wraps STATIC)
add_library(desktop-app::linux_glibc_wraps ALIAS linux_glibc_wraps)

get_filename_component(src_loc . REALPATH)

nice_target_sources(linux_glibc_wraps ${src_loc}
PRIVATE
    platform/linux/linux_glibc_wraps.c
    platform/linux/linux_glibc_wraps_32.c
    platform/linux/linux_glibc_wraps_64.c
)
if (NOT build_linux32)
    set_source_files_properties(${src_loc}/platform/linux/linux_glibc_wraps_32.c PROPERTIES HEADER_FILE_ONLY TRUE)
else()
    set_source_files_properties(${src_loc}/platform/linux/linux_glibc_wraps_64.c PROPERTIES HEADER_FILE_ONLY TRUE)
endif()
