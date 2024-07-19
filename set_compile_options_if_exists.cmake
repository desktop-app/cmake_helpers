# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

include(CheckCXXCompilerFlag)

function(set_compile_options_if_exists var_name)
    set(var_name "")
    foreach (entry ${ARGN})
        string(MAKE_C_IDENTIFIER ${entry} entry_identifier)
        check_cxx_compiler_flag(${entry} DESKTOP_APP_${entry_identifier}_EXISTS)
        if (DESKTOP_APP_${entry_identifier}_EXISTS)
            list(APPEND var_name ${entry})
        endif()
    endforeach()
endfunction()
