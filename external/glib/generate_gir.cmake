# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(generate_gir target_name namespace deps src_target_name)
    find_program(DESKTOP_APP_GIRSCANNER g-ir-scanner REQUIRED)

    set(gen_dst ${CMAKE_CURRENT_BINARY_DIR}/gen)
    file(MAKE_DIRECTORY ${gen_dst})

    set(gen_timestamp ${gen_dst}/${target_name}_gir.timestamp)
    set(gen_file ${gen_dst}/${target_name}.gir)

    add_custom_command(
    OUTPUT
        ${gen_timestamp}
    BYPRODUCTS
        ${gen_file}
    COMMAND
        env
        $<$<BOOL:${DESKTOP_APP_ASAN}>:LDFLAGS=-fsanitize=address>
        ${DESKTOP_APP_GIRSCANNER}
        --quiet
        -n
        ${namespace}
        --nsversion
        1.0
        -l
        $<TARGET_FILE:${src_target_name}>
        --c-include
        "$<JOIN:$<FILTER:$<TARGET_PROPERTY:${src_target_name},SOURCES>,INCLUDE,\\.h$>,$<SEMICOLON>--c-include>"
        -i
        "$<JOIN:${deps},$<SEMICOLON>-i>"
        -o
        ${gen_file}
        "$<TARGET_PROPERTY:${src_target_name},SOURCES>"
    COMMAND
        echo 1> ${gen_timestamp}
    COMMAND_EXPAND_LISTS
    COMMENT "Generating GIR (${target_name})"
    DEPENDS
        ${DESKTOP_APP_GIRSCANNER}
        ${src_target_name}
    )
    generate_target(${target_name} gir ${gen_timestamp} "${gen_file}" ${gen_dst})
endfunction()
