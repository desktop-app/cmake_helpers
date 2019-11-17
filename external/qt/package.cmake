# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

set(qt_version 5.12.5)

if (WIN32)
    set(qt_loc ${libs_loc}/Qt-${qt_version})
else()
endif()

set(Qt5_DIR ${qt_loc}/lib/cmake/Qt5)

find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

if (LINUX)
    find_package(Qt5 COMPONENTS DBus)
endif()

set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "(gen)")
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "(gen)")
