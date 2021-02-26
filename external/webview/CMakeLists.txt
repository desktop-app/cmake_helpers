# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_webview INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_webview ALIAS external_webview)

add_library(external_webview_bundled INTERFACE)

set(webview_loc ${third_party_loc}/webview)

if (WIN32)
    target_include_directories(external_webview_bundled
    INTERFACE
        ${webview_loc}/script/microsoft.web.webview2.1.0.664.37/build/native/include
    )

    if (build_win64)
        set(webview_arch x64)
    else()
        set(webview_arch x86)
    endif()
    target_link_libraries(external_webview_bundled
    INTERFACE
        ${webview_loc}/script/microsoft.web.webview2.1.0.664.37/build/native/${webview_arch}/WebView2LoaderStatic.lib
    )
endif()

target_include_directories(external_webview_bundled
INTERFACE
    ${webview_loc}
)

target_link_libraries(external_webview
INTERFACE
    external_webview_bundled
)
