#!/usr/bin/bash

# ----------- USER CUSTOM -------------
libdir="lib"
outdir="bin"
bin="$outdir/app"
includedirs="include include/external"  # libraries separated by commas
srcdir="src"
# -------------------------------------

linked_libs="-lraylib -lopengl32 -lgdi32 -lwinmm"
other_flags="-O1 -Wall -std=c99 -Wimplicit-function-declaration"

function check_exit_code {
	local failure="Operation failed. Exit code $?"
	local success="Completed successfully"

	if [ -n "$1" ]; then
		failure="Failed to $1. Exit code $?"
		success="$1 successfull"
	fi

	if [ $? -eq 0 ]; then
		echo $success
	else
		echo $failure
	fi
}

function print_help {
        printf "Simple Raylib Build&Run Utility\n\n"
        printf "ray [COMMAND] [OPTIONS]\n\n"
        echo   "  help"
        echo   "                -- print this"
        echo   "  clean"
        echo   "                -- cleans the output directory"
        echo   "  build"
        echo   "                -- compiles and links .c files. Produces an executable at bin/ directory"
        echo   "  run"
        echo   "                -- runs the executable"
        echo   "  bun"
        echo   "                -- builds the project and run the executable"
}

function clean {
	echo Clean started...

	if [ ! -d $outdir ] || [ -z $(ls $outdir) ]; then
        echo Binaries not found. Cleaning cancelled
        return 0
	else
        rm $outdir/*
		check_exit_code "Clean"
	fi
}

function build {
	echo Build started...

	local cfiles=$(find ${srcdir}/*.c)

	if [ -z "$cfiles" ]; then
		echo No .c file found
		echo Nothing to compile though
		return 1
	fi

	if [ ! -d "$outdir" ]; then
		echo Creating bin folder
		mkdir $outdir
	fi

	include_flags=$(printf -- '-I %s ' $includedirs)

	# Compile the source files with GCC
	if gcc $cfiles -o $bin $other_flags -L $libdir/ $linked_libs $include_flags; then
		echo Build successfully
	else
		echo Build failed
	fi
}

function run {
	echo Checking for exucutable...

    if [ ! -f $bin ]; then
        echo "Executable not found. Build your project at first"
        return 1
    fi

    echo Launching executable...
        
	./$bin "$@"
    check_exit_code "Run"
}

case $1 in
	help) 
		print_help;;
	clean) 
		clean;;
	build) 
		clean
		build;;
	run) 
		run "${@:2}";;
	bun)
		clean
		build
		run "${@:2}";;
	*)
		printf "Command not recognized. For a list of commands type:\ray help\n"
		exit 1;;
esac