#!/bin/bash -E
# -*- mode:sh; tab-width:4; indent-tabs-mode:t; -*-
# vim: set filetype=bash syntax=sh autoindent noexpandtab tabstop=4 shiftwidth=4 :
trap 'echo -e "\nAborted due to unhandled error $? in $0 near line $LINENO on the following line:\n$BASH_COMMAND" 1>&2; exit 70' ERR

# Defaults
STRIP="strip"
STRIPFLAGS="--strip-debug"
COMPILE=""
SETVERINFOFLAGS=""
KEEPDEBUG=false

# Print usage
usage()
{
cat <<EOF
do_link -s <strip-cmd> <link-cmd>
  Perform linking using the supplied command. Depending on target arch it will produce both stripped and unstripped binaries, add md5sum and/or run tests.

Where options are:
  -s <strip-cmd>
    Command used for stripping binaries. Will determine target platform and do appropriate actions.
    The default is "$STRIP" which only should be used when not cross-compiling.
  -c <gcc-cmd>
    Command used for compiling NOISREV version info file generated by genbuildver.sh which will then be linked with the produced output executable.
    Default is to not add this version info to the output executable, supplying this parameter will trigger it.
  -d
    Do not strip debug-info, will call cp instead of strip.
  -f <strip-flags>
    Strip flags used with <strip-cmd>. Default is "$STRIPFLAGS".
EOF
}

# Main function called last in script with all input parameters.
main ()
{
	local target=""
	local debugelf=""
	local component=""

	# Parse options
	while getopts "hc:s:v:f:d" flag; do
		case $flag in
			h ) usage; exit 0;;
			c ) COMPILE="$OPTARG";;
			s ) STRIP="$OPTARG";;
			f ) STRIPFLAGS="$OPTARG";;
			d ) KEEPDEBUG=true;;
			* ) echo "Unknown option"; exit 64;;
		esac
	done
	shift $(($OPTIND - 1))
	OPTIND=1

	# Check arguments
	if [ $# -eq 0 ]; then
		usage
		exit 64
	fi

	# Grab command line to run for linking
	local command=("$@")

	# Find elf position in link command and extract/modify name
	local idx=0
	for arg in $@
	do
		[[ "$arg" = "-o" ]] && elf_idx=$((idx + 1 )) && break
		idx=$((idx + 1))
	done
	elf=${command[elf_idx]}

	target="linux"

	# Append .unstripped to name except for linux which always contain debug info
	[ "$target" == "linux" ] && debugelf=$elf || debugelf=${elf}.unstripped

	# Update output executable name in command line
	command[$elf_idx]=$debugelf

	# Create version info file if compile command is supplied
	if [ -n "$COMPILE" ]
	then
		component=${elf##*/}
		component=${component%%_*}
		echo "Creating $component NOISREV version info"
		genbuildver $component ${elf}_ver.c
		$COMPILE -c ${elf}_ver.c -o ${elf}_ver.o
		command+=("${elf}_ver.o")
	fi

	# Do actual linking
	echo "Linking $elf"
	"${command[@]}"

	# Create unstripped/stripped binaries with version info appended according to all rules
	if [[ "$debugelf" == "$elf" ]]
	then
		echo "Appending version info to $target executable"
	#	setverinfo $SETVERINFOFLAGS $elf #piaoyimq
	else
		if $KEEPDEBUG
		then
			echo "Keeping debug info and appending version info to $target executables"
			cp $debugelf $elf
		else
			echo "Stripping debug info and appending version info to $target executables"
			$STRIP $STRIPFLAGS -o $elf $debugelf
		fi
#		setverinfo $SETVERINFOFLAGS $elf#piaoyimq
#		setverinfo $SETVERINFOFLAGS $debugelf#piaoyimq
	fi

	# Remove created temporary version files
	[ -n "$COMPILE" ] && rm -f ${elf}_ver.c ${elf}_ver.o

	# Success, regardless of exit code from previous command
	return 0
}

# Call main function with all parameters quoted.
main "$@"
