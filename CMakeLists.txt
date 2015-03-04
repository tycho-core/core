cmake_minimum_required (VERSION 2.8)

set(core_link_libs "")
if(ty_platform MATCHES "pc")
    set(core_link_libs "dbghelp;winmm")
elseif(ty_platform MATCHES "osx")
	set(core_link_libs "/usr/lib/libncurses.dylib")
endif()

tycho_add_library(core "${core_link_libs}" "libs")



