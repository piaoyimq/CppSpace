#!/bin/bash
# -*- mode:sh; tab-width:4; indent-tabs-mode:t; -*-
# vim: set filetype=bash syntax=sh autoindent noexpandtab tabstop=4 shiftwidth=4 :

# This is a wrapper for gcc/binutils that supports specifying locations of these tools and a target sysroot.
# Note that binutils needs to be configured with "--with-sysroot" to support a target sysroot.
# Currently only support 64bit binaries to keep a short lib-search-path. 

# Tools selection
GCC="/proj/epg-tools/gcc/4.8.1"
BINUTILS="/proj/epg-tools/binutils/2.21.1.gold.rhel5"
SYSROOT="/proj/epg-tools/sysroot/rhel5.6"
WRAPPER="/proj/epg-tools/compilers/gcc4.8.1-rhel5.6-binutils2.21.1.gold"

# Grab actual command from link name and put wrapper commands first in path for gcc to be able to call ld correctly.
CMD="${0##*/}"
export PATH="$WRAPPER/bin:$PATH"

# Now run wanted command with --sysroot supplied to supported commands
case "$CMD" in
	gcc|g++|cpp|c++)
		exec $GCC/bin/$CMD --sysroot "$SYSROOT" "$@"
		;;
	cc)
		exec $GCC/bin/gcc --sysroot "$SYSROOT" "$@"
		;;
	gcov)
		exec $GCC/bin/$CMD "$@"
		;;
	ld|ld.bfd|ld.gold)
		exec $BINUTILS/bin/$CMD --sysroot "$SYSROOT" -rpath "$GCC/lib64" -rpath "$SYSROOT/lib64rt" "$@"
		;;
	addr2line|ar|as|c++filt|elfedit|gprof|nm|objcopy|objdump|ranlib|readelf|size|strings|strip)
		exec $BINUTILS/bin/$CMD "$@"
		;;
	*)
		echo "Unknown command $CMD used as link name, aborting!"
		echo "This script is supposed to be used as a gcc wrapper by linking gcc/binutils commands to it..."
		exit 64
		;;
esac

