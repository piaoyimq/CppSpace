#!/bin/bash

# Generate a .c-file containing version info symbol "bin_version_raw" to embed into an executable.
# Required by SEOS SDK for linkingusing the SEOS genver.sh as input.
# $1 - Target executable
# $2 - Output .c-file


[ -z "$VERSION_INFO" -a -n "$BUILD_ID" ] && VERSION_INFO="$BUILD_ID"
[ -z "$VERSION_INFO" -a -n "$CIMS_JOB_NAME" ] && VERSION_INFO="Private build: $CIMS_JOB_NAME"
[ -z "$VERSION_INFO" -a -n "$BUILD_OUTPUT_DIR" ] && VERSION_INFO="Private build: `basename $BUILD_OUTPUT_DIR`"
[ -z "$VERSION_INFO" ] && VERSION_INFO="Private build: `git rev-parse --abbrev-ref HEAD`"

BIN_VERSION_TAG="Build info:"
TIMESTAMP=`date -R`
BASELINE=`git log --abbrev-commit|sed -n '1p;1q'` #piaoyimq
BUILDER="$USER"
MACHINE=`uname -n`
COPYRIGHT=`date '+,Copyright (C) 2010-%Y, piaoyimq. All rights reserved.'`

#echo "char bin_version_raw[512] = \"${BIN_VERSION_TAG} \"" > $2
echo "const char* bin_version_raw = \"${BIN_VERSION_TAG} \"" > $2
echo "\"$1 Version ${VERSION_INFO}, \"" >> $2
echo "\"latest commit: ${BASELINE}, \"" >> $2
echo "\"built by ${BUILDER}@${MACHINE} at ${TIMESTAMP} \"" >> $2
echo "\"${COPYRIGHT}\";" >> $2
