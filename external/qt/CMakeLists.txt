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

set(common_qt_libs
    qwebp
    qgif
    qjpeg
    Qt5PrintSupport
    Qt5AccessibilitySupport
    Qt5FontDatabaseSupport
    Qt5EventDispatcherSupport
    Qt5ThemeSupport
    Qt5Network
    Qt5Widgets
    Qt5Gui
    qtharfbuzz
    qtlibpng
)

if (WIN32)
    set(qt_libs
        ${common_qt_libs}
        Qt5Core
        Qt5WindowsUIAutomationSupport
        qtmain
        qwindows
        qtfreetype
        qtpcre2
    )
    set(qt_libs_list "")
    foreach(lib ${qt_libs})
        list(APPEND qt_libs_list "${lib}$<$<CONFIG:Debug>:d>")
    endforeach()
else()
endif()

target_link_directories(external_qt
INTERFACE
    ${qt_loc}/lib
    ${qt_loc}/plugins
    ${qt_loc}/plugins/bearer
    ${qt_loc}/plugins/platforms
    ${qt_loc}/plugins/imageformats
)

target_link_libraries(external_qt
INTERFACE
    ${qt_libs_list}
)
