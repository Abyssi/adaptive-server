#!/bin/bash

# get the absolute path of the executable
SELF_PATH=$(cd -P -- "$(dirname -- "$0")" && pwd -P) && SELF_PATH=$SELF_PATH/$(basename -- "$0")

# resolve symlinks
while [[ -h $SELF_PATH ]]; do
    # 1) cd to directory of the symlink
    # 2) cd to the directory of where the symlink points
    # 3) get the pwd
    # 4) append the basename
    DIR=$(dirname -- "$SELF_PATH")
    SYM=$(readlink "$SELF_PATH")
    SELF_PATH=$(cd "$DIR" && cd "$(dirname -- "$SYM")" && pwd)/$(basename -- "$SYM")
done

 DIR_PATH=$(dirname "$SELF_PATH")
 
 CMDLINE_ARGS=""
for i in "$@"
do CMDLINE_ARGS+=" $i"
done

cd $DIR_PATH/bin
./adaptiveserver$CMDLINE_ARGS

# Example arguments -p 8080 -c 50 -l 4

