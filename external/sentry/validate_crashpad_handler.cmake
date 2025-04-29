# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(validate_crashpad_handler_error text)
    if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
        message(FATAL_ERROR ${text})
    else()
        message(WARNING ${text})
    endif()
endfunction()

function(validate_crashpad_handler target_name)
    if (build_win64)
        set(modules_subdir x64)
    else()
        set(modules_subdir x86)
    endif()
    set(modules_hash_loc ${CMAKE_BINARY_DIR}/modules/${modules_subdir})
    set(modules_debug_loc ${CMAKE_BINARY_DIR}/Debug/modules/${modules_subdir})
    set(modules_release_loc ${CMAKE_BINARY_DIR}/Release/modules/${modules_subdir})

    set(key_path ${modules_hash_loc}/crashpad/crashpad_handler)
    set(module_debug_path ${modules_debug_loc}/crashpad)
    set(module_release_path ${modules_release_loc}/crashpad)

    set(key "")
    if (EXISTS ${key_path})
        file(READ ${key_path} key)
    endif()

    set(handler_debug ${sentry_lib_loc}/Debug/crashpad_build/handler/crashpad_handler.exe)
    set(handler_release ${sentry_lib_loc}/Release/crashpad_build/handler/crashpad_handler.exe)

    find_package(Python3 REQUIRED)
    execute_process(
    COMMAND
        ${Python3_EXECUTABLE}
        ${cmake_helpers_loc}/external/sentry/validate_crashpad_handler.py
        ${handler_debug}
    OUTPUT_VARIABLE key_debug
    ERROR_VARIABLE error
    )
    if (NOT "${error}" STREQUAL "")
        validate_crashpad_handler_error(${error})
        return()
    endif()

    execute_process(
    COMMAND
        ${Python3_EXECUTABLE}
        ${cmake_helpers_loc}/external/sentry/validate_crashpad_handler.py
        ${handler_release}
    OUTPUT_VARIABLE key_release
    ERROR_VARIABLE error
    )
    if (NOT "${error}" STREQUAL "")
        validate_crashpad_handler_error(${error})
        return()
    endif()

    if (NOT "${key_debug}${key_release}" STREQUAL "${key}")
        file(MAKE_DIRECTORY ${module_debug_path})
        file(COPY ${handler_debug} DESTINATION ${module_debug_path})

        file(MAKE_DIRECTORY ${module_release_path})
        file(COPY ${handler_release} DESTINATION ${module_release_path})

        file(MAKE_DIRECTORY ${modules_hash_loc}/crashpad)
        file(WRITE ${key_path} ${key_debug}${key_release})
    endif()

    target_compile_definitions(${target_name}
    PRIVATE
        DESKTOP_APP_CRASHPAD_HANDLER_HASH=$<IF:$<CONFIG:Debug>,${key_debug},${key_release}>
    )

    target_link_libraries(${target_name}
    PRIVATE
        desktop-app::external_openssl
    )
endfunction()
