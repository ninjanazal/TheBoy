#!/usr/bin/env bash


# - - - - - - - - - -
# Holds the base global path
# - - - - - - - - - -
CURRENT_PATH=$PWD


# - - - - - - - - - -
# Marks if the build files should be deleted
# - - - - - - - - - -
CLEAR_ALL=FALSE


# - - - - - - - - - -
# Marks the current target generator
# - - - - - - - - - -
BUILD_TARGET="mingw"


# - - - - - - - - - -
# Marks if should run the build output
# - - - - - - - - - -
RUN_CONFIG=FALSE


# - - - - - - - - - -
# Marks if should run using verbose mode
# - - - - - - - - - -
VERBOSE="FALSE"


# - - - - - - - - - - - - - - - - - - - -
# FUNCTIONS - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - -


# - - - - - - - - - -
# - - - - - - - - - -
PrintHelp () {
	echo -e "______________________________________________________________________"
	echo -e "Build help for the project                                           |" 
	echo -e "_____________________________________________________________________|"
	echo -e "*-r (--run)       | Will run the compilation result                  |"
	echo -e "*-c (--clearall)  | Will clear the previous compilation and rebuild  |"
	echo -e "*-v (--verbose)   | Will use a explicit output                       |"
	echo -e "*-t (--target=)   | Changes the target generator                     |"
	echo -e "   |--> Available: mingw                                             |"
	echo -e "   |--> Available: vs                                                |"
	echo -e "_____________________________________________________________________|"
}


# - - - - - - - - - -
# Script START
# - - - - - - - - - -
echo -e "[SCRIPT] Regenerating the project"



# - - - - - - - - - -
# Parameters and current project layout
# - - - - - - - - - -
for i in "$@"; do
	case $i in
		-c|--clearall)
			CLEAR_ALL=TRUE
			shift
			;;
		-t=*|--target=*)
			# Parse target values
			targ="${i#*=}"
			case $targ in
				"mingw")
					BUILD_TARGET=$targ
					;;
				"vs")
					BUILD_TARGET=$targ
					;;
			esac
			shift
			;;
		-r|--run)
			RUN_CONFIG=TRUE
			shift
			;;
		-v|--verbose)
			VERBOSE="true"
			shift
			;;
		-h|--help)
			PrintHelp
			exit 0
	esac
done


if [ "$CLEAR_ALL" = TRUE ]; then
	echo -e "[SCRIPT] Cleaning up"
		rm Build -R -rf
		rm out -R -rf
fi



# - - - - - - - - - -
# Base Project Generation & Compilation
# - - - - - - - - - -
echo -e "[SCRIPT] Moving to TheBoy Files at "$PWD"!"
if [ ! -d "Build" ]; then
	mkdir Build
fi
cd Build
echo -e "[SCRIPT] Generating compilation Project at "$PWD""

case $BUILD_TARGET in
	"mingw")
		cmake -G "MinGW Makefiles" -DVERBOSE=${VERBOSE} -DTARGETCONFIG="mingw" ..
		;;
	"vs")
		cmake -G "Visual Studio 17 2022" -DVERBOSE=${VERBOSE} -DTARGETCONFIG="vs" ..
		;;
esac

echo -e "[SCRIPT] Building Project"
cmake --build .
cd ..


# - - - - - - - - - -
# Parameter for run after build
# - - - - - - - - - -
if [ "$RUN_CONFIG" = TRUE ]; then
	echo -e "[SCRIPT] Running compilation\n - - - - -\n"
	./Build/src/TheBoy.exe
fi
