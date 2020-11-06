# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_zlib INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_zlib ALIAS external_zlib)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(ZLIB REQUIRED)
    target_link_libraries(external_zlib INTERFACE ZLIB::ZLIB)
elseif (WIN32)
    target_compile_definitions(external_zlib INTERFACE ZLIB_WINAPI)
    target_include_directories(external_zlib INTERFACE ${libs_loc}/zlib)
    if (build_win64)
        set(zlib_config_folder x64)
    else()
        set(zlib_config_folder x86)
    endif()
    set(zlib_lib_loc ${libs_loc}/zlib/contrib/vstudio/vc14/${zlib_config_folder}/ZlibStat$<IF:$<CONFIG:Debug>,Debug,ReleaseWithoutAsm>)
    target_link_libraries(external_zlib INTERFACE ${zlib_lib_loc}/zlibstat.lib)
elseif (APPLE AND NOT build_osx)
    target_link_libraries(external_zlib INTERFACE /usr/local/macos/lib/libz.a)
elseif (APPLE)
    target_link_libraries(external_zlib INTERFACE /usr/local/lib/libz.a)
else()
    target_link_libraries(external_zlib INTERFACE z)
endif()
