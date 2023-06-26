# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

include(${cmake_helpers_loc}/external/glib/generate_gir.cmake)
include(${cmake_helpers_loc}/external/glib/generate_cppgir.cmake)

function(generate_dbus target_name interface_prefix namespace interface_file)
    find_program(DESKTOP_APP_GDBUSCODEGEN gdbus-codegen REQUIRED)

    set(gen_loc ${CMAKE_CURRENT_BINARY_DIR}/gen)
    set(gen_dst ${gen_loc}/dbus)
    file(MAKE_DIRECTORY ${gen_dst})

    set(gen_timestamp ${gen_dst}/${target_name}_${namespace}_dbus.timestamp)
    set(gen_files
        ${gen_dst}/${target_name}_${namespace}_dbus.c
        ${gen_dst}/${target_name}_${namespace}_dbus.h
    )

    add_custom_command(
    OUTPUT
        ${gen_timestamp}
    BYPRODUCTS
        ${gen_files}
    COMMAND
        ${DESKTOP_APP_GDBUSCODEGEN}
        --interface-prefix
        ${interface_prefix}
        --generate-c-code
        ${gen_dst}/${target_name}_${namespace}_dbus
        --c-namespace
        ${namespace}
        --c-generate-object-manager
        ${interface_file}
    COMMAND
        echo 1> ${gen_timestamp}
    COMMENT "Generating D-Bus C code for ${namespace} (${target_name})"
    DEPENDS
        ${DESKTOP_APP_GDBUSCODEGEN}
        ${interface_file}
    )

    add_library(${target_name}_${namespace}_dbus STATIC)
    init_target(${target_name}_${namespace}_dbus "(gen)")
    target_link_libraries(${target_name}_${namespace}_dbus PUBLIC desktop-app::external_glib)
    generate_target(${target_name}_${namespace}_dbus timestamp ${gen_timestamp} "${gen_files}" ${gen_loc})

    add_library(${target_name}_${namespace} INTERFACE)
    init_target_folder(${target_name}_${namespace} "(gen)")
    target_link_libraries(${target_name}_${namespace} INTERFACE ${target_name}_${namespace}_dbus)
    generate_gir(${target_name}_${namespace} ${namespace} Gio-2.0 ${target_name}_${namespace}_dbus)
    generate_cppgir(${target_name}_${namespace} ${CMAKE_CURRENT_BINARY_DIR}/gen/${target_name}_${namespace}.gir)
    add_dependencies(${target_name}_${namespace}_cppgir ${target_name}_${namespace}_gir)

    get_target_property(target_type ${target_name} TYPE)
    if (${target_type} STREQUAL "INTERFACE_LIBRARY")
        target_link_libraries(${target_name} INTERFACE ${target_name}_${namespace})
    else()
        target_link_libraries(${target_name} PUBLIC ${target_name}_${namespace})
    endif()
endfunction()
