# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_crash_reports INTERFACE IMPORTED GLOBAL)

if (WIN32)
    add_subdirectory(breakpad)
    target_link_libraries(external_crash_reports
    INTERFACE
        external_breakpad
    )
else()
    target_link_libraries(external_crash_reports
    INTERFACE
        external_crashpad
    )
endif()
