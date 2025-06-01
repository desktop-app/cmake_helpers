# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_kcoreaddons INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_kcoreaddons ALIAS external_kcoreaddons)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(KF${QT_VERSION_MAJOR}CoreAddons QUIET)
    if (KF${QT_VERSION_MAJOR}CoreAddons_FOUND)
        target_link_libraries(external_kcoreaddons INTERFACE KF${QT_VERSION_MAJOR}::CoreAddons)
        return()
    endif()
endif()

add_library(external_kcoreaddons_bundled STATIC)
init_target(external_kcoreaddons_bundled "(external)")

set(kcoreaddons_loc ${third_party_loc}/kcoreaddons)
set(kcoreaddons_src ${kcoreaddons_loc}/src/lib)

nice_target_sources(external_kcoreaddons_bundled ${kcoreaddons_src}
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
    remove_target_sources(external_kcoreaddons_bundled ${kcoreaddons_src}
        util/kshell.cpp
        util/kshell.h
        util/kshell_p.h
        util/kshell_unix.cpp
        util/kuser.h
        util/kuser_unix.cpp
    )
endif()

if (LINUX AND TARGET Qt::DBus)
    set_source_files_properties(${kcoreaddons_src}/io/org.freedesktop.portal.FileTransfer.xml PROPERTIES INCLUDE ${kcoreaddons_src}/io/dbustypes_p.h)
    qt_add_dbus_interface(_dbus_SRCS ${kcoreaddons_src}/io/org.freedesktop.portal.FileTransfer.xml org.freedesktop.portal.FileTransfer)

    set_source_files_properties(${kcoreaddons_src}/io/org.kde.KIOFuse.VFS.xml PROPERTIES NO_NAMESPACE TRUE)
    qt_add_dbus_interface(_dbus_SRCS ${kcoreaddons_src}/io/org.kde.KIOFuse.VFS.xml org.kde.KIOFuse.VFS)

    target_sources(external_kcoreaddons_bundled PRIVATE ${_dbus_SRCS})

    target_compile_definitions(external_kcoreaddons_bundled
    PRIVATE
        HAVE_QTDBUS
    )
endif()

target_compile_definitions(external_kcoreaddons_bundled
PUBLIC
    KCOREADDONS_EXPORT=
PRIVATE
    ACCOUNTS_SERVICE_ICON_DIR="/var/lib/AccountsService/icons"
)

target_include_directories(external_kcoreaddons_bundled SYSTEM
PUBLIC
    ${kcoreaddons_src}/io
    ${kcoreaddons_src}/util
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/public
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/private
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(external_kcoreaddons_bundled
PRIVATE
    desktop-app::external_qt
)

target_link_libraries(external_kcoreaddons
INTERFACE
    external_kcoreaddons_bundled
)
