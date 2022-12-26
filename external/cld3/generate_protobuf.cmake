function(generate_single_protobuf target_name gen_dst protobuf_name executable)
    file(MAKE_DIRECTORY ${gen_dst})

    # Copied from myprotobuf.cmake.
    if (PROTOBUF_GENERATE_CPP_APPEND_PATH)
        # Create an include path for each file specified
        set(FIL ${cld3_src}/${protobuf_name})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(ABS_PATH ${ABS_FIL} PATH)
        list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
        if (${_contains_already} EQUAL -1)
            list(APPEND _protobuf_include_path -I ${ABS_PATH})
        endif()
    else()
        set(_protobuf_include_path -I ${cld3_src})
    endif()

    if (DEFINED PROTOBUF_IMPORT_DIRS)
        foreach (DIR ${PROTOBUF_IMPORT_DIRS})
            get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if (${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif()
        endforeach()
    endif()
    #

    get_filename_component(protobuf_name_we ${protobuf_name} NAME_WE)

    set(gen_timestamp ${gen_dst}/${protobuf_name}.timestamp)
    set(gen_files
        ${gen_dst}/${protobuf_name_we}.pb.cc
        ${gen_dst}/${protobuf_name_we}.pb.h
    )

    set(gen_src ${cld3_src}/${protobuf_name})
    add_custom_command(
    OUTPUT
        ${gen_timestamp}
    BYPRODUCTS
        ${gen_files}
    COMMAND
        ${executable}
        --cpp_out
        ${gen_dst}
        ${_protobuf_include_path}
        ${gen_src}
    COMMAND
        echo 1> ${gen_timestamp}
    COMMENT "Generating protobuf ${protobuf_name} (${target_name})"
    DEPENDS
        ${executable}
        ${gen_src}
    VERBATIM
    )
    generate_target(${target_name} ${protobuf_name} ${gen_timestamp} "${gen_files}" ${gen_dst})
endfunction()
