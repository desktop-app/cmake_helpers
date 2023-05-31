# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(target_add_resource target_name)
    set(list ${ARGN})
    target_sources(${target_name} PRIVATE ${list})
    get_target_property(existing_resources ${target_name} RESOURCE)
    if (NOT "${existing_resources}" STREQUAL "existing_resources-NOTFOUND")
        foreach(existing ${existing_resources})
            list(APPEND list ${existing})
        endforeach()
    endif()
    set_target_properties(${target_name} PROPERTIES RESOURCE "${list}")
endfunction()

function(target_prepare_qrc target_name)
    if (NOT APPLE)
        set_target_properties(${target_name} PROPERTIES AUTORCC ON)
    else()
        set(rcc_flags --binary "$<TARGET_PROPERTY:${target_name},AUTORCC_OPTIONS>")

        set(qrc_files)
        get_target_property(list ${target_name} SOURCES)
        foreach (entry ${list})
            get_source_file_property(skip_autogen ${entry} SKIP_AUTOGEN)
            if (NOT ${entry} MATCHES "\\.qrc$" OR skip_autogen)
                continue()
            endif()
            list(APPEND qrc_files ${entry})
        endforeach()
        if (NOT qrc_files)
            return()
        endif()
        set(rcc_file ${target_name}.rcc)
        set(rcc_path "${CMAKE_BINARY_DIR}/${rcc_file}")
        source_group(TREE ${CMAKE_BINARY_DIR} PREFIX Resources FILES ${rcc_path})
        add_custom_command(OUTPUT ${rcc_path}
            DEPENDS ${qrc_files}
            COMMAND Qt::rcc ${rcc_flags} -o ${rcc_path} ${qrc_files}
            COMMAND_EXPAND_LISTS VERBATIM
        )
        target_add_resource(${target_name} ${rcc_path})
    endif()
endfunction()
