# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(validate_d3d_error text)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
        message(FATAL_ERROR ${text})
    else()
        message(WARNING ${text})
    endif()
endfunction()

function(validate_d3d_compiler target_name)
    if (build_win64)
        set(modules_subdir x64)
    else()
        set(modules_subdir x86)
    endif()
    set(modules_hash_loc ${CMAKE_BINARY_DIR}/modules/${modules_subdir})
    set(modules_debug_loc ${CMAKE_BINARY_DIR}/Debug/modules/${modules_subdir})
    set(modules_release_loc ${CMAKE_BINARY_DIR}/Release/modules/${modules_subdir})
    
    set(key_path ${modules_hash_loc}/d3d/d3dcompiler_47)
    set(module_debug_path ${modules_debug_loc}/d3d)
    set(module_release_path ${modules_release_loc}/d3d)

    set(key "")
    if (EXISTS ${key_path})
        file(READ ${key_path} key)
    endif()

    if (NOT "$ENV{WindowsSdkDir}" STREQUAL "")
        set(windows_sdk_loc $ENV{WindowsSdkDir} CACHE INTERNAL "Windows SDK Path" FORCE)
    endif()

    string(LENGTH "${key}" key_length)
    if (NOT "${key_length}" STREQUAL "32"
        OR NOT EXISTS ${module_debug_path}/d3dcompiler_47.dll
        OR NOT EXISTS ${module_release_path}/d3dcompiler_47.dll)
        
        if ("${windows_sdk_loc}" STREQUAL "")
            validate_d3d_error("Could not find Windows SDK.")
            return()
        endif()
        set(sdk_compiler ${windows_sdk_loc}/Redist/D3D/${modules_subdir}/d3dcompiler_47.dll)

        find_package(Python REQUIRED)
        execute_process(
        COMMAND
            ${Python_EXECUTABLE}
            ${cmake_helpers_loc}/validate_d3d_compiler.py
            ${sdk_compiler}
        OUTPUT_VARIABLE key
        ERROR_VARIABLE error
        )
        if (NOT "${error}" STREQUAL "")
            validate_d3d_error(${error})
            return()
        endif()

        file(MAKE_DIRECTORY ${modules_debug_loc}/d3d)
        file(COPY ${sdk_compiler} DESTINATION ${module_debug_path})

        file(MAKE_DIRECTORY ${modules_release_loc}/d3d)
        file(COPY ${sdk_compiler} DESTINATION ${module_release_path})

        file(MAKE_DIRECTORY ${modules_hash_loc}/d3d)    
        file(WRITE ${key_path} ${key})
    endif()

    target_compile_definitions(${target_name}
    PRIVATE
        DESKTOP_APP_D3DCOMPILER_HASH=${key}
    )

    target_link_libraries(${target_name}
    PRIVATE
        desktop-app::external_openssl
    )
endfunction()
