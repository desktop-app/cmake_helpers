# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_subdirectory(auto_updates)
add_subdirectory(crash_reports)
add_subdirectory(expected)
add_subdirectory(ffmpeg)
add_subdirectory(gsl)
add_subdirectory(iconv)
add_subdirectory(lz4)
add_subdirectory(openal)
add_subdirectory(openssl)
add_subdirectory(opus)
add_subdirectory(qt)
add_subdirectory(qr_code_generator)
add_subdirectory(ranges)
add_subdirectory(rlottie)
if (APPLE)
    add_subdirectory(sp_media_key_tap)
endif()
add_subdirectory(variant)
add_subdirectory(xxhash)
add_subdirectory(zlib)
