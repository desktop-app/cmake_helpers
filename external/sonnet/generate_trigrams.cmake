function(generate_trigrams target_name trigrams_file)
    set(gen_dst ${CMAKE_BINARY_DIR}/data)
    file(MAKE_DIRECTORY ${gen_dst})

    set(gen_timestamp ${gen_dst}/${target_name}_trigrams.timestamp)
    set(gen_files ${gen_dst}/trigrams.map)

    add_custom_command(
    OUTPUT
        ${gen_timestamp}
    BYPRODUCTS
        ${gen_files}
    COMMAND
        external_sonnet_parsetrigrams "${trigrams_file}" > "${gen_files}"
    COMMAND
        echo 1> ${gen_timestamp}
    COMMENT "Generating trigrams (${target_name})"
    DEPENDS
        external_sonnet_parsetrigrams
        ${trigrams_file}
    )
    generate_target(${target_name} trigrams ${gen_timestamp} "${gen_files}" ${gen_dst})
endfunction()
