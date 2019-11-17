# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(force_include target_name file_path)
    if (MSVC)
        target_compile_options(${target_name}
        PRIVATE
            /FI${file_path}
        )
    else()
        target_compile_options(${target_name}
        PRIVATE
            -include ${file_path}
        )
    endif()
endfunction()
