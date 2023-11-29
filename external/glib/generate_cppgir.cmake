# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(generate_cppgir target_name gir)
    set(cppgir_loc ${cmake_helpers_loc}/external/glib/cppgir)

    # cppgir generates all the dependent headers everytime, better to have a global folder
    set(gen_dst ${CMAKE_BINARY_DIR}/gen)
    file(MAKE_DIRECTORY ${gen_dst})

    set(gen_timestamp ${gen_dst}/${target_name}_cppgir.timestamp)

    set(ignore_files
        ${cppgir_loc}/data/cppgir.ignore
        ${cppgir_loc}/data/cppgir_unix.ignore
    )

    set(gir_path)
    if (IS_ABSOLUTE "${gir}")
        set(gir_path ${gir})
    endif()

    add_custom_command(
    OUTPUT
        ${gen_timestamp}
    COMMAND
        CppGir::cppgir
        --debug
        1
        --class
        --class-full
        --expected
        --optional
        --ignore
        "$<JOIN:${ignore_files},:>"
        --output
        ${gen_dst}
        ${gir}
    COMMAND
        echo 1> ${gen_timestamp}
    COMMENT "Generating C++ wrapper for ${gir} (${target_name})"
    DEPENDS
        CppGir::cppgir
        ${ignore_files}
        ${gir_path}
    )
    generate_target(${target_name} cppgir ${gen_timestamp} "" ${gen_dst})

    get_target_property(target_type ${target_name} TYPE)
    if (${target_type} STREQUAL "INTERFACE_LIBRARY")
        target_link_libraries(${target_name} INTERFACE CppGir::gi)
        target_compile_definitions(${target_name} INTERFACE GI_INLINE)
    else()
        target_link_libraries(${target_name} PUBLIC CppGir::gi)
        target_compile_definitions(${target_name} PUBLIC GI_INLINE)
    endif()
endfunction()
