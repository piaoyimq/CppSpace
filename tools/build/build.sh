#!/bin/bash
#-E
# -*- mode:sh; tab-width:4; indent-tabs-mode:t; -*-
# vim: set filetype=bash syntax=sh autoindent noexpandtab tabstop=4 shiftwidth=4 :

# A build wrapper calling CMake and ninja supporting standardized variant folders and start_job.

set -e
export WS_ROOT=$HOME/workspace/CppSpace

# Setup shell
trap 'echo -e "\nAborted due to unhandled error $? in $0 near line $LINENO on the following line:\n$BASH_COMMAND" 1>&2; exit 70' ERR
shopt -s extglob # Support fancy globbing

# Global/env defaults
[ -n "$BUILD_DIR_CMAKE" ] || export BUILD_DIR_CMAKE="$WS_ROOT/output"
[ -n "$JOB_CORES" ] || export JOB_CORES=16
[ "$BUILD_WITH_GCOV" == true ] || export BUILD_WITH_GCOV=false
VALID_PLATFORMS="Linux_x86|Linux_mips"
declare -A SHORT_PLATFORMS=( [lin]=Linux_x86 [mips]=Linux_mips)
VALID_VARIANTS="debug|gcov|gprof|asan|sec|automock|"
CONFIGS="Linux_x86"
COMPONENTS=""
CMAKEFLAGS=""
NINJAFLAGS=""
CLEAN=false
FORCE_BUILD=false
FORCE_GENERATE=false
PRINT_TARGETS=false
RUN_TESTCASE=false

# Print usage
usage()
{
cat <<EOF
${0##*/} [<options>] [<components/targets>]

Will generate CMake files and build specified <components/targets> for the specified configuration. The default is to generate CMake files for Linux_x86 if not already existing and then always call ninja to build.

Specifying a configuration with variant, for example, will pass the appropriate flags "-DUSE_x' flags to CMake and use the standard build folder for the configuration.

Where <options> are:
 -c <configurations>
    Build comma-separated configurations, using the <platform>[.<variant>] form. Default is Linux_x86.
    Valid platforms: $VALID_PLATFORMS (${!SHORT_PLATFORMS[@]})
    Valid variants: $VALID_VARIANTS
 -g
    Force generation, skip build unless -b is also specified.
 -G
    cmake Generators, using "Ninja"|"Unix Makefiles", default is "Unix Makefiles"
 -b
    Build, skip generation unless -g is also specified.
 -D
    Clean, delete build folder contents before generate/build.
 -p
    Print valid components/targets.
 -t
    Run test case after build finished.
 -v
    Verbose, more output from CMake and ninja.
 -x
    Print debug info when running each line of this script.
 -h
    Show this help text.

Examples:
${0##*/} # Generate and build all of Linux_x86
${0##*/} -c Linux_x86.debug # Build app for Linux_x86 keeping symbols without optimization (-DUSE_DEBUG=1)
${0##*/} -c Linux_x86.gcov cpw_Linux_x86.elf # Build cpw for Linux_x86 with coverage support (-DUSE_COVERAGE=1)
${0##*/} clean # Call ninja clean target for Linux_x86

EOF
}

# Exit with error, $1 exit code (/usr/include/sysexits.h), $2 text output
err_exit()
{
	echo -e "\nError: $2\nAborting..." >&2
	exit $1
}

run_unit()
{
    set +e 
    file_list=$(ls $BUILD_DIR_CMAKE/$config/bin|grep Test)
	echo $file_list
	echo "$BUILD_DIR_CMAKE/$config/bin"
    array=($file_list)
    
    length=${#array[@]}
    if [ $length -ne 0 ]
    then
        for ((i=0; i<$length; i++))
            do
    	    echo
    	    echo -e "\033[33m**********  Perform [$i] ${array[$i]}  **********\033[0m"
    	    echo
				cd $WS_ROOT
                ./output/$config/bin/${array[$i]}
            done
    fi

}

# Clean, generate and build for config $1
build_config()
{
	local config="$1"
	local cmakeflags="$CMAKEFLAGS"
	local ninjaflags="$NINJAFLAGS"
	local targets=""
	local skipped=""
	local dir=""
	local platform=""
	local variant=""

	# Validate configuration=platform[.variant] and set build directory
	platform="${config%%.*}"
	[[ "$config" =~ '.' ]] && variant="${config##*.}"
	[[ "$platform" == @($VALID_PLATFORMS) ]] || platform=${SHORT_PLATFORMS[$platform]}
	[[ "$platform" == @($VALID_PLATFORMS) ]] || err_exit 64 "Invalid configuration/platform $config"
	[[ "$variant" == @($VALID_VARIANTS) ]] || err_exit 64 "Invalid variant $variant"
	[ -z "$variant" ] && config=$platform || config=$platform.$variant
	dir="$BUILD_DIR_CMAKE/$config"

	# Map variant to CMake flags
	case "$variant" in
		*gcov*) cmakeflags+=" -DUSE_COVERAGE=1";;
		*debug*) cmakeflags+=" -DUSE_DEBUG=1";;
		*sec*)  cmakeflags+=" -DUSE_SEC=1";;
		*automock*) cmakeflags+=" -DUSE_AUTOMOCK=1";;
	esac

	# Special case for build_all building with coverage in Linux_x86 folder
	if $BUILD_WITH_GCOV && [ "$config" == "Linux_x86" ]
	then
		cmakeflags+=" -DUSE_COVERAGE=1"
	fi

	# Translate/filter components to targets
	for cmp in ${COMPONENTS[@]}
	do
		case "$cmp" in
			all|clean|build.ninja|help|test|gtest|*_${platform}*) # Unmodified targets
				targets+=" $cmp"
				;;
			*@(${VALID_PLATFORMS/|/*|*})*) # Skipping components for other platform
				skipped+=" $cmp"
				;;
			*) # Append current platform and elf to component
				targets+=" ${cmp}_${platform}.elf"
				;;
		esac
	done
	if [ -n "$skipped" ] && [ -z "$targets" ]
	then
		echo "All specified components/targets were skipped ($skipped) for this configuration ($config), doing nothing!"
		return 0
	fi

	# Clean? If clean specified and build directory exists
	if $CLEAN
	then
		if [ -d "$dir" ]
		then
			echo "Cleaning build directory ${dir}..."
			rm -rf "$dir" \
				|| err_exit 73 "Failed cleaning build dir $dir"
		fi
		#echo "Cleaning xxx.."
		#make -C ${WS_ROOT}/tools/build clean-xxx-${platform}
	fi

	# Generate? If generate specified OR not building only and no build.ninja already generated.
	if $FORCE_GENERATE || ( ! $FORCE_BUILD && [ ! -e "$dir/build.ninja" ] )
	then
		mkdir -p "$dir" \
			|| err_exit 73 "Failed creating build dir $dir"
		cd "$dir"

		echo "Generating \"$GENERATOR\" files in $dir using CMake..."
		cmake -G "$GENERATOR" -DCMAKE_TOOLCHAIN_FILE=$WS_ROOT/tools/build/cmake/platforms/${platform}.cmake $cmakeflags\
			$WS_ROOT || err_exit 78 "Failed generating CMake files in $dir"
	fi

	# Print targets and return?
	if $PRINT_TARGETS
	then
		echo "Targets/components for $config:"
		ninja -C $dir -t targets
		return 0
	fi

	# Build? If build specified OR not generate only.
	if $FORCE_BUILD || ! $FORCE_GENERATE
	then
		#echo "Building xxx.."
		#make -C ${WS_ROOT}/tools/build xxx-${platform}

		if [ "Ninja" == "$GENERATOR" ]
		then
			#if use ninja, you can open the following lines
			echo "Building using ninja in ${dir}..."
			[ -z "$targets" ] || echo "Building targets:$targets"
			[ -z "$skipped" ] || echo "Skipping invalid targets for this platform:$skipped"
			ninja -j $JOB_CORES -C $dir $ninjaflags $targets \
				|| err_exit 65 "Failed building in $dir"
		else
			#if use make, you can open this line
			echo "Building using make in ${dir}..."
			make -f $dir/Makefile #or use make -C $dir
		fi

    	if $RUN_TESTCASE
    	then
    		echo "Run unit test:"
    		run_unit
    		return 0
    	fi
	fi
}

# Main function called last in script with all input parameters.
main ()
{
	local config
	GENERATOR="Unix Makefiles"
	# Parse options
	while getopts "c:bgG:Dptvxh" flag
	do
		case $flag in
			c ) CONFIGS="${OPTARG//,/ }";;
			b ) FORCE_BUILD=true;;
			g ) FORCE_GENERATE=true;;
			G ) GENERATOR=$OPTARG;;
			D ) CLEAN=true;;
			p ) PRINT_TARGETS=true;;
			t ) RUN_TESTCASE=true;;
			v ) CMAKEFLAGS+=" -LAH"
				NINJAFLAGS+=" -v -d keeprsp"
				;;
			x ) set -x;;
			h ) usage; exit 0;;
			* ) echo "Unknown option"; exit 64;;
		esac
	done

	shift $(($OPTIND - 1))
	OPTIND=1

	# Grab parameters and check environment
	COMPONENTS="$@"
	echo "$WS_ROOT"
	[ -d "$WS_ROOT" ] || err_exit 66 "Please setup your CppSpace git workspace"
	[ -n "$BUILD_DIR_CMAKE" ] || err_exit 66 "Not a valid CMake build directory"

	# Loop through specified configurations and build
	for config in $CONFIGS
	do
		build_config $config
	done
}

# Call main function with all parameters quoted. Last with NO newline after to make live updates of running script work better. Watch out for editors adding it back.
main "$@"
