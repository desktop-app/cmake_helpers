# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

{
  'includes': [
    '../../common/common.gypi',
  ],
  'targets': [{
    'target_name': 'linux_glibc_wraps',
    'type': 'static_library',
    'variables': {
      'src_loc': '.',
    },
    'configurations': {
      'Debug': {
      },
      'Release': {
      },
    },
    'sources': [
      '<(src_loc)/linux_glibc_wraps.c',
    ],
    'conditions': [[ '"<!(uname -m)" == "x86_64" or "<!(uname -m)" == "aarch64"', {
      'sources': [
        '<(src_loc)/linux_glibc_wraps_64.c',
      ],
    }, {
      'sources': [
        '<(src_loc)/linux_glibc_wraps_32.c',
      ],
    }]],
  }],
}
