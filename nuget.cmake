# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(nuget_add_package package_name package package_version)
    get_property(nuget_exe_defined GLOBAL PROPERTY nuget_exe_path_property SET)
    if (NOT nuget_exe_defined)
        # Thanks https://github.com/clarkezone/flutter_win_webview/blob/master/webview_popupauth/windows/CMakeLists.txt
        find_program(NUGET_EXE NAMES nuget PATHS "${CMAKE_SOURCE_DIR}/../ThirdParty/NuGet")
        if (NOT NUGET_EXE)
            message("NUGET.EXE not found.")
            message(FATAL_ERROR "Please install this executable, and run CMake again.")
        endif()
        message("Resolved NuGet executable: ${NUGET_EXE}")
        set_property(GLOBAL PROPERTY nuget_exe_path_property ${NUGET_EXE})
    else()
        get_property(NUGET_EXE GLOBAL PROPERTY nuget_exe_path_property)
    endif()

    set(package_key nuget_${package_name}_version_property)
    get_property(package_version_defined GLOBAL PROPERTY ${package_key} SET)
    if (NOT package_version_defined)
        set(packages_loc ${CMAKE_BINARY_DIR}/packages)
        file(MAKE_DIRECTORY ${packages_loc})

        set_property(GLOBAL PROPERTY ${package_key} ${package_version})
        execute_process(
        COMMAND
            ${NUGET_EXE}
            install
            ${package}
            -Version ${package_version}
            -ExcludeVersion
            -OutputDirectory ${packages_loc}
        )
    else()
        get_property(package_version_cached GLOBAL PROPERTY ${package_key})
        if (NOT (${package_version_cached} EQUAL ${package_version}))
            message(FATAL_ERROR "Package ${package_name} requested with both ${package_version_cached} and ${package_version}")
        endif()
    endif()
    set(${package_name}_loc ${CMAKE_BINARY_DIR}/packages/${package} PARENT_SCOPE)
endfunction()

function(nuget_add_webview target_name)
    nuget_add_package(webview2 "Microsoft.Web.WebView2" 1.0.1901.177)

    set(webview2_loc_native ${webview2_loc}/build/native)
    # target_link_libraries(${target_name}
    # PRIVATE
    #     ${webview2_loc_native}/Microsoft.Web.WebView2.targets
    #     ${src_loc}/ForceStaticLink.targets
    # )
    #
    # This works, but just to be sure, manually link to static library.
    #
    target_include_directories(${target_name}
    PRIVATE
        ${webview2_loc_native}/include
    )
    if (build_win64)
        set(webview2_lib_folder x64)
    else()
        set(webview2_lib_folder x86)
    endif()
    target_link_libraries(${target_name}
    PRIVATE
        ${webview2_loc_native}/${webview2_lib_folder}/WebView2LoaderStatic.lib
    )
endfunction()

function(nuget_add_winrt target_name)
    nuget_add_package(winrt "Microsoft.Windows.CppWinRT" 2.0.210505.3)

    set(gen_dst ${CMAKE_BINARY_DIR}/packages/gen)
    file(MAKE_DIRECTORY ${gen_dst}/winrt)

    set(winrt_sdk_version)
    if (NOT DEFINED CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
        set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
    endif()
    # https://gitlab.kitware.com/cmake/cmake/-/blob/89cfb90b9c0893133983b4f25896671c4f07497c/Modules/InstallRequiredSystemLibraries.cmake#L381
    if (";${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION};$ENV{UCRTVersion};$ENV{WindowsSDKVersion};" MATCHES [=[;([0-9.]+)[;\]]=])
        set(winrt_sdk_version ${CMAKE_MATCH_1})
    endif()
    set(winrt_version_key ${gen_dst}/winrt/version_key)
    set(winrt_version_test ${winrt_version_key}_test)
    set(sdk_version_key ${gen_dst}/winrt/sdk_version_key)
    set(sdk_version_test ${sdk_version_key}_test)

    execute_process(
    COMMAND
        ${winrt_loc}/bin/cppwinrt
        /?
    OUTPUT_FILE
        ${winrt_version_test}
    COMMAND_ERROR_IS_FATAL ANY
    )
    file(WRITE ${sdk_version_test} ${winrt_sdk_version})
    execute_process(
    COMMAND
        ${CMAKE_COMMAND} -E compare_files ${winrt_version_key} ${winrt_version_test}
        RESULT_VARIABLE winrt_version_compare_result
    )
    execute_process(
    COMMAND
        ${CMAKE_COMMAND} -E compare_files ${sdk_version_key} ${sdk_version_test}
        RESULT_VARIABLE sdk_version_compare_result
    )
    if (winrt_version_compare_result EQUAL 0 AND sdk_version_compare_result EQUAL 0)
        message("Using existing WinRT headers.")
    else()
        message("Generating new WinRT headers.")

        execute_process(
        COMMAND
            ${winrt_loc}/bin/cppwinrt
            -input ${winrt_sdk_version}
            -output ${gen_dst}
        COMMAND_ERROR_IS_FATAL ANY
        )
        file(RENAME ${winrt_version_test} ${winrt_version_key})
        file(RENAME ${sdk_version_test} ${sdk_version_key})
    endif()

    target_include_directories(${target_name}
    PRIVATE
        ${gen_dst}
    )
endfunction()
