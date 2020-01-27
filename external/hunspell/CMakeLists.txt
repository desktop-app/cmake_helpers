# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_hunspell OBJECT)
add_library(desktop-app::external_hunspell ALIAS external_hunspell)
init_target(external_hunspell "(external)")

set(hunspell_loc ${third_party_loc}/hunspell)
set(hunspell_src ${hunspell_loc}/src)

nice_target_sources(external_hunspell ${hunspell_src}
PRIVATE
	hunspell/affentry.cxx
	hunspell/affentry.hxx
	hunspell/affixmgr.cxx
	hunspell/affixmgr.hxx
	hunspell/atypes.hxx
	hunspell/baseaffix.hxx
	hunspell/csutil.cxx
	hunspell/csutil.hxx
	hunspell/filemgr.cxx
	hunspell/filemgr.hxx
	hunspell/hashmgr.cxx
	hunspell/hashmgr.hxx
	hunspell/htypes.hxx
	hunspell/hunspell.cxx
	hunspell/hunspell.h
	hunspell/hunspell.hxx
	hunspell/hunvisapi.h
	hunspell/hunzip.cxx
	hunspell/hunzip.hxx
	hunspell/phonet.cxx
	hunspell/phonet.hxx
	hunspell/replist.cxx
	hunspell/replist.hxx
	hunspell/suggestmgr.cxx
	hunspell/suggestmgr.hxx
	hunspell/utf_info.hxx
)

target_include_directories(external_hunspell
PUBLIC
    ${hunspell_src}
)

target_compile_definitions(external_hunspell
PUBLIC
	HUNSPELL_STATIC
)
