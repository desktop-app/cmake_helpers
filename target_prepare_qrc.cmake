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
        set(qrc_resources)
        foreach (qrc_file ${qrc_files})
            # Entries inside the build tree are other rules' outputs and are
            # skipped below, so a generated .qrc may name their sources instead.
            get_source_file_property(qrc_generated_from "${qrc_file}" QRC_GENERATED_FROM)
            if (qrc_generated_from)
                list(APPEND qrc_resources ${qrc_generated_from})
            endif()
            if (NOT EXISTS "${qrc_file}")
                continue()
            endif()
            get_filename_component(qrc_folder "${qrc_file}" DIRECTORY)
            file(READ "${qrc_file}" qrc_content)
            string(REGEX MATCHALL "<file[^>]*>[^<]*</file>" qrc_entries "${qrc_content}")
            foreach (qrc_entry ${qrc_entries})
                string(REGEX REPLACE "^<file[^>]*>(.*)</file>$" "\\1" qrc_relative "${qrc_entry}")
                get_filename_component(qrc_resource "${qrc_folder}/${qrc_relative}" ABSOLUTE)
                string(FIND "${qrc_resource}" "${CMAKE_BINARY_DIR}/" qrc_in_binary)
                if (NOT EXISTS "${qrc_resource}" OR qrc_in_binary EQUAL 0)
                    continue()
                endif()
                list(APPEND qrc_resources "${qrc_resource}")
            endforeach()
        endforeach()
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${qrc_files})
        set(rcc_file ${target_name}.rcc)
        set(rcc_path "${CMAKE_BINARY_DIR}/${rcc_file}")
        source_group(TREE ${CMAKE_BINARY_DIR} PREFIX Resources FILES ${rcc_path})
        add_custom_command(OUTPUT ${rcc_path}
            DEPENDS ${qrc_files} ${qrc_resources}
            COMMAND Qt::rcc ${rcc_flags} -o ${rcc_path} ${qrc_files}
            COMMAND_EXPAND_LISTS VERBATIM
        )
        target_add_resource(${target_name} ${rcc_path})
    endif()
endfunction()
