# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_qt ALIAS external_qt)

if (LINUX)
    if (NOT build_linux32)
        target_compile_definitions(external_qt INTERFACE Q_OS_LINUX64)
    else()
        target_compile_definitions(external_qt INTERFACE Q_OS_LINUX32)
    endif()
endif()

if (DESKTOP_APP_USE_PACKAGED)
    target_link_libraries(external_qt
    INTERFACE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network)

    if (Qt5DBus_FOUND)
        target_link_libraries(external_qt INTERFACE Qt5::DBus)
    endif()

    target_include_directories(external_qt
    INTERFACE
        ${Qt5Core_PRIVATE_INCLUDE_DIRS}
        ${Qt5Gui_PRIVATE_INCLUDE_DIRS})
else()
    target_include_directories(external_qt SYSTEM
    INTERFACE
        ${qt_loc}/include
        ${qt_loc}/include/QtCore
        ${qt_loc}/include/QtGui
        ${qt_loc}/include/QtCore/${qt_version}
        ${qt_loc}/include/QtGui/${qt_version}
        ${qt_loc}/include/QtCore/${qt_version}/QtCore
        ${qt_loc}/include/QtGui/${qt_version}/QtGui
    )

    if (Qt5DBus_FOUND)
        target_include_directories(external_qt SYSTEM
        INTERFACE
            ${qt_loc}/include/QtDBus
        )
    endif()

    target_compile_definitions(external_qt
    INTERFACE
        _REENTRANT
        QT_STATICPLUGIN
        QT_PLUGIN
        QT_WIDGETS_LIB
        QT_NETWORK_LIB
        QT_GUI_LIB
        QT_CORE_LIB
    )

    if (WIN32)
        set(qt_lib_prefix "")
        set(qt_lib_suffix $<$<CONFIG:Debug>:d>.lib)
    else()
        set(qt_lib_prefix lib)
        if (APPLE)
            set(qt_lib_suffix $<$<CONFIG:Debug>:_debug>.a)
        else()
            set(qt_lib_suffix .a)
        endif()
    endif()

    set(common_qt_libs
        plugins/imageformats/${qt_lib_prefix}qwebp
        plugins/imageformats/${qt_lib_prefix}qgif
        plugins/imageformats/${qt_lib_prefix}qjpeg
        lib/${qt_lib_prefix}Qt5PrintSupport
        lib/${qt_lib_prefix}Qt5AccessibilitySupport
        lib/${qt_lib_prefix}Qt5FontDatabaseSupport
        lib/${qt_lib_prefix}Qt5EventDispatcherSupport
        lib/${qt_lib_prefix}Qt5ThemeSupport
        lib/${qt_lib_prefix}Qt5Network
        lib/${qt_lib_prefix}Qt5Widgets
        lib/${qt_lib_prefix}Qt5Gui
        lib/${qt_lib_prefix}qtharfbuzz
        lib/${qt_lib_prefix}qtlibpng
    )

    set(qt_libs_list "")
    if (WIN32)
        set(qt_libs
            ${common_qt_libs}
            lib/${qt_lib_prefix}Qt5Core
            lib/${qt_lib_prefix}Qt5WindowsUIAutomationSupport
            lib/${qt_lib_prefix}qtmain
            lib/${qt_lib_prefix}qtfreetype
            lib/${qt_lib_prefix}qtpcre2
            plugins/platforms/${qt_lib_prefix}qwindows
        )
        foreach (lib ${qt_libs})
            list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
        endforeach()
    elseif (APPLE)
        if (NOT build_osx)
            set(qt_libs
                ${common_qt_libs}
                lib/${qt_lib_prefix}Qt5Core
                lib/${qt_lib_prefix}Qt5GraphicsSupport
                lib/${qt_lib_prefix}Qt5ClipboardSupport
                lib/${qt_lib_prefix}qtfreetype
                lib/${qt_lib_prefix}qtpcre2
                plugins/platforms/${qt_lib_prefix}qcocoa
                plugins/bearer/${qt_lib_prefix}qgenericbearer
            )
        else()
            set(qt_libs
                lib/${qt_lib_prefix}Qt5PrintSupport
                lib/${qt_lib_prefix}Qt5PlatformSupport
                lib/${qt_lib_prefix}Qt5Network
                lib/${qt_lib_prefix}Qt5Widgets
                lib/${qt_lib_prefix}Qt5Gui
                lib/${qt_lib_prefix}Qt5Core
                lib/${qt_lib_prefix}qtharfbuzzng
                lib/${qt_lib_prefix}qtfreetype
                lib/${qt_lib_prefix}qtpcre
                plugins/platforms/${qt_lib_prefix}qcocoa
                plugins/imageformats/${qt_lib_prefix}qwebp
                plugins/bearer/${qt_lib_prefix}qgenericbearer
            )
        endif()
        foreach (lib ${qt_libs})
            list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
        endforeach()
        target_link_libraries(external_qt
        INTERFACE
            desktop-app::external_zlib
            cups
        )
    else()
        if (Qt5DBus_FOUND)
            set(qt_libs_dbus lib/${qt_lib_prefix}Qt5DBus)
        endif()
        set(qt_libs
            plugins/platforminputcontexts/${qt_lib_prefix}composeplatforminputcontextplugin
            plugins/platforminputcontexts/${qt_lib_prefix}ibusplatforminputcontextplugin
            plugins/platforminputcontexts/${qt_lib_prefix}fcitxplatforminputcontextplugin
            plugins/platforminputcontexts/${qt_lib_prefix}himeplatforminputcontextplugin
            plugins/platforminputcontexts/${qt_lib_prefix}nimfplatforminputcontextplugin
            plugins/platforms/${qt_lib_prefix}qxcb
            lib/${qt_lib_prefix}Qt5XcbQpa
            lib/${qt_lib_prefix}Qt5LinuxAccessibilitySupport
            lib/${qt_lib_prefix}Qt5ServiceSupport
            lib/${qt_lib_prefix}Qt5EdidSupport
            plugins/bearer/${qt_lib_prefix}qconnmanbearer
            plugins/bearer/${qt_lib_prefix}qgenericbearer
            plugins/bearer/${qt_lib_prefix}qnmbearer
            ${common_qt_libs}
            ${qt_libs_dbus}
            lib/${qt_lib_prefix}Qt5Core
            lib/${qt_lib_prefix}qtpcre2
            lib/${qt_lib_prefix}xcb-static
        )
        foreach (lib ${qt_libs})
            list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
        endforeach()
    endif()

    target_link_libraries(external_qt
    INTERFACE
        ${qt_libs_list}
    )

    if (LINUX)
        target_include_directories(external_qt SYSTEM
        INTERFACE
            ${qt_loc}/mkspecs/linux-g++
        )
        target_link_options(external_qt
        INTERFACE
            -static-libstdc++
            -pthread
            -rdynamic
        )
        if (DESKTOP_APP_USE_GLIBC_WRAPS)
            target_link_options(external_qt
            INTERFACE
                -Wl,-wrap,aligned_alloc
                -Wl,-wrap,secure_getenv
                -Wl,-wrap,clock_gettime
                -Wl,--no-as-needed,-lrt
            )
            if (NOT build_linux32)
                target_link_options(external_qt
                INTERFACE
                    -Wl,-wrap,__divmodti4
                )
            else()
                target_link_options(external_qt
                INTERFACE
                    -Wl,-wrap,__divmoddi4
                )
            endif()
        endif()
        target_link_static_libraries(external_qt
        INTERFACE
            SM
            ICE
            fontconfig
            freetype
            expat
            z
            xcb-shm
            xcb-xfixes
            xcb-render
            xkbcommon
            xkbcommon-x11
            Xrender
            icutu
            icui18n
            icuuc
            icudata
        )
        if (DESKTOP_APP_USE_GLIBC_WRAPS)
            target_link_libraries(external_qt
            INTERFACE
                desktop-app::linux_glibc_wraps
                $<TARGET_FILE:desktop-app::linux_glibc_wraps>
            )
        endif()
        target_link_libraries(external_qt
        INTERFACE
            xcb
            X11
            X11-xcb
            dl
            glib-2.0
            pthread
        )
        find_library(DBUS_LIBRARY dbus-1)
        if (DBUS_LIBRARY)
            target_link_libraries(external_qt INTERFACE dbus-1)
        endif()
    endif()
endif()
