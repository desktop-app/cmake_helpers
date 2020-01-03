# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_crash_reports INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_crash_reports ALIAS external_crash_reports)

if (NOT DESKTOP_APP_DISABLE_CRASH_REPORTS)
    if (WIN32 OR LINUX OR build_macstore)
        add_subdirectory(breakpad)
        target_link_libraries(external_crash_reports
        INTERFACE
            desktop-app::external_breakpad
        )
    else()
        add_subdirectory(crashpad)
        target_link_libraries(external_crash_reports
        INTERFACE
            desktop-app::external_crashpad
        )
    endif()
endif()
