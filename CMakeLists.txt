# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_subdirectory(external)
if (LINUX
    AND NOT DESKTOP_APP_USE_PACKAGED
    AND Qt5WaylandClient_FOUND)
    add_subdirectory(linux_wayland_helper)
endif()
if (LINUX AND NOT DESKTOP_APP_USE_PACKAGED)
    add_subdirectory(linux_glib_helper)
endif()
if (LINUX AND NOT DESKTOP_APP_USE_PACKAGED)
    add_subdirectory(linux_gtk_helper)
endif()
if (DESKTOP_APP_USE_ALLOCATION_TRACER)
    add_subdirectory(linux_allocation_tracer)
endif()
if (WIN32)
    add_subdirectory(win_directx_helper)
endif()
