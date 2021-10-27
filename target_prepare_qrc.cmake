# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(target_add_resource target_name)
    set(list ${ARGV})
    list(REMOVE_AT list 0)
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

        set(qrc_files_all "$<FILTER:$<TARGET_PROPERTY:${target_name},SOURCES>,INCLUDE,\\.qrc$>")
        if (WIN32)
            set(qrc_files "$<FILTER:${qrc_files_all},EXCLUDE,(mac|darwin|osx|linux|posix)\\.>")
        elseif (APPLE)
            set(qrc_files "$<FILTER:${qrc_files_all},EXCLUDE,(win|linux)\\.>")
        elseif (LINUX)
            set(qrc_files "$<FILTER:${qrc_files_all},EXCLUDE,(win|mac|darwin|osx)\\.>")
        endif()
        set(rcc_file ${target_name}.rcc)
        set(rcc_path "${CMAKE_BINARY_DIR}/${rcc_file}")
        source_group(TREE ${CMAKE_BINARY_DIR} PREFIX Resources FILES ${rcc_path})
        if (DESKTOP_APP_QT6)
            set(rcc_binary Qt6::rcc)
        else()
            set(rcc_binary Qt5::rcc)
        endif()
        add_custom_command(OUTPUT ${rcc_path}
            DEPENDS ${qrc_files}
            COMMAND ${rcc_binary} ${rcc_flags} -o ${rcc_path} ${qrc_files}
            COMMAND_EXPAND_LISTS VERBATIM
        )
        target_add_resource(${target_name} ${rcc_path})
    endif()
endfunction()
