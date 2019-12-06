# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_subdirectory(external)
if (DESKTOP_APP_USE_GLIBC_WRAPS)
    add_subdirectory(linux_glibc_wraps)
endif()
