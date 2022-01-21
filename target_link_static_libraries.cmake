# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(target_link_static_libraries target_name)
    set(list ${ARGV})
    list(REMOVE_AT list 0)

    set(writing_now "")
    set(private_libs "")
    set(public_libs "")
    set(interface_libs "")
    foreach (entry ${list})
        if (${entry} STREQUAL "PRIVATE" OR ${entry} STREQUAL "PUBLIC" OR ${entry} STREQUAL "INTERFACE")
            set(writing_now ${entry})
        else()
            set(link_option "-Wl,--push-state,-Bstatic,-l${entry},--pop-state")
            if ("${writing_now}" STREQUAL "PRIVATE")
                list(APPEND private_libs ${link_option})
            elseif ("${writing_now}" STREQUAL "PUBLIC")
                list(APPEND public_libs ${link_option})
            elseif ("${writing_now}" STREQUAL "INTERFACE")
                list(APPEND interface_libs ${link_option})
            else()
                message(FATAL_ERROR "Unknown frameworks scope for target ${target_name}")
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
