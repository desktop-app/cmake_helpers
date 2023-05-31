# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(generate_implib)
    find_package(Python3 REQUIRED)

    set(gen_dst ${CMAKE_CURRENT_BINARY_DIR}/gen)
    file(MAKE_DIRECTORY ${gen_dst})

    foreach (lib ${ARGN})
        string(MAKE_C_IDENTIFIER ${lib} lib_identifier)
        find_library(DESKTOP_APP_${lib_identifier}_PATH ${lib} REQUIRED)
        get_filename_component(lib_file ${DESKTOP_APP_${lib_identifier}_PATH} NAME)

        set(gen_timestamp ${gen_dst}/implib_${lib_identifier}.timestamp)
        set(gen_files
            ${gen_dst}/${lib_file}.init.c
            ${gen_dst}/${lib_file}.tramp.S
        )

        add_custom_command(
        OUTPUT
            ${gen_timestamp}
        BYPRODUCTS
            ${gen_files}
        COMMAND
            ${Python3_EXECUTABLE}
            ${cmake_helpers_loc}/external/Implib.so/implib-gen.py
            -o
            ${gen_dst}
            ${DESKTOP_APP_${lib_identifier}_PATH}
        COMMAND
            echo 1> ${gen_timestamp}
        COMMENT "Generating dlopen helper for ${lib_file}"
        DEPENDS
            ${DESKTOP_APP_${lib_identifier}_PATH}
        )

        add_library(implib_${lib_identifier} STATIC)
        init_target(implib_${lib_identifier} "(gen)")
        generate_target(implib_${lib_identifier} timestamp ${gen_timestamp} "${gen_files}" ${gen_dst})
    endforeach()
endfunction()

function(target_link_optional_libraries target_name)
    set(writing_now "")
    set(private_libs "")
    set(public_libs "")
    set(interface_libs "")
    foreach (entry ${ARGN})
        if (${entry} STREQUAL "PRIVATE" OR ${entry} STREQUAL "PUBLIC" OR ${entry} STREQUAL "INTERFACE")
            set(writing_now ${entry})
        else()
            string(MAKE_C_IDENTIFIER ${entry} entry_identifier)
            if (NOT TARGET implib_${entry_identifier})
                generate_implib(${entry})
            endif()
            set(result
                $<LINK_ONLY:implib_${entry_identifier}>
                $<TARGET_FILE:implib_${entry_identifier}>
            )
            if ("${writing_now}" STREQUAL "PRIVATE")
                list(APPEND private_libs ${result})
            elseif ("${writing_now}" STREQUAL "PUBLIC")
                list(APPEND public_libs ${result})
            elseif ("${writing_now}" STREQUAL "INTERFACE")
                list(APPEND interface_libs ${result})
            else()
                message(FATAL_ERROR "Unknown libraries scope for target ${target_name}")
            endif()
        endif()
    endforeach()

    if (NOT "${public_libs}" STREQUAL "")
        target_link_libraries(${target_name} PUBLIC ${public_libs})
    endif()
    if (NOT "${private_libs}" STREQUAL "")
        target_link_libraries(${target_name} PRIVATE ${private_libs})
    endif()
    if (NOT "${interface_libs}" STREQUAL "")
        target_link_libraries(${target_name} INTERFACE ${interface_libs})
    endif()
endfunction()
