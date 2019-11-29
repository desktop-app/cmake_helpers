# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_qt INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_qt ALIAS external_qt)

if (LINUX)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        target_compile_definitions(external_qt INTERFACE Q_OS_LINUX64)
    else()
        target_compile_definitions(external_qt INTERFACE Q_OS_LINUX32)
    endif()
endif()

target_include_directories(external_qt SYSTEM
INTERFACE
    ${qt_loc}/include
    ${qt_loc}/include/QtCore
    ${qt_loc}/include/QtGui
    ${qt_loc}/include/QtDBus
    ${qt_loc}/include/QtCore/${qt_version}
    ${qt_loc}/include/QtGui/${qt_version}
    ${qt_loc}/include/QtCore/${qt_version}/QtCore
    ${qt_loc}/include/QtGui/${qt_version}/QtGui
)

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
    set(qt_lib_suffix $<$<CONFIG:Debug>:_debug>.a)
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
    endif()
    foreach (lib ${qt_libs})
        list(APPEND qt_libs_list "${qt_loc}/${lib}${qt_lib_suffix}")
    endforeach()
    target_link_libraries(external_qt
    INTERFACE
        desktop-app::external_zlib
        cups
    )
endif()

target_link_libraries(external_qt
INTERFACE
    ${qt_libs_list}
)
