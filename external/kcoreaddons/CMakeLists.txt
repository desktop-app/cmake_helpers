# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED AND QT_VERSION_MAJOR EQUAL 5)
    add_library(external_kcoreaddons INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_kcoreaddons ALIAS external_kcoreaddons)

    find_package(KF5CoreAddons)
    if (KF5CoreAddons_FOUND)
        target_link_libraries(external_kcoreaddons INTERFACE KF5::CoreAddons)
        return()
    endif()
endif()

add_library(external_kcoreaddons STATIC)
add_library(desktop-app::external_kcoreaddons ALIAS external_kcoreaddons)
init_target(external_kcoreaddons "(external)")

set(kcoreaddons_loc ${third_party_loc}/kcoreaddons)
set(kcoreaddons_src ${kcoreaddons_loc}/src/lib)

nice_target_sources(external_kcoreaddons ${kcoreaddons_src}
PRIVATE
    io/kurlmimedata.cpp
    io/kurlmimedata.h
    util/ksandbox.cpp
    util/ksandbox.h
    util/kshell.cpp
    util/kshell.h
    util/kshell_p.h
    util/kshell_unix.cpp
    util/kuser.h
    util/kuser_unix.cpp
)

if (NOT LINUX)
    remove_target_sources(external_kcoreaddons ${kcoreaddons_src}
        util/kshell.cpp
        util/kshell.h
        util/kshell_p.h
        util/kshell_unix.cpp
        util/kuser.h
        util/kuser_unix.cpp
    )
endif()

if (NOT DESKTOP_APP_DISABLE_DBUS_INTEGRATION)
    set_source_files_properties(${kcoreaddons_src}/io/org.freedesktop.portal.FileTransfer.xml PROPERTIES INCLUDE ${kcoreaddons_src}/io/dbustypes_p.h)
    qt_add_dbus_interface(_dbus_SRCS ${kcoreaddons_src}/io/org.freedesktop.portal.FileTransfer.xml org.freedesktop.portal.FileTransfer)

    set_source_files_properties(${kcoreaddons_src}/io/org.kde.KIOFuse.VFS.xml PROPERTIES NO_NAMESPACE TRUE)
    qt_add_dbus_interface(_dbus_SRCS ${kcoreaddons_src}/io/org.kde.KIOFuse.VFS.xml org.kde.KIOFuse.VFS)

    target_sources(external_kcoreaddons PRIVATE ${_dbus_SRCS})

    target_compile_definitions(external_kcoreaddons
    PRIVATE
        HAVE_QTDBUS
    )
endif()

target_compile_definitions(external_kcoreaddons
PUBLIC
    KCOREADDONS_EXPORT=
PRIVATE
    ACCOUNTS_SERVICE_ICON_DIR="/var/lib/AccountsService/icons"
)

target_include_directories(external_kcoreaddons SYSTEM
PUBLIC
    ${kcoreaddons_src}/io
    ${kcoreaddons_src}/util
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/public
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/private
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(external_kcoreaddons
PRIVATE
    desktop-app::external_qt
)
