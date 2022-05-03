#!/usr/bin/env bash


# - - - - - - - - - -
# Target SFML repo with branch
# - - - - - - - - - -
SFML_GIT="https://github.com/SFML/SFML.git -b 2.5.x"


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


# - - - - - - - - - - - - - - - - - - - -
# FUNCTIONS - - - - - - - - - - - - - - -
# - - - - - - - - - - - - - - - - - - - -


# - - - - - - - - - -
# Builds the sfml lib
# - - - - - - - - - -
BuildSFML () {
	echo -e "[SCRIPT] Building SFML Module!"
	if [ ! -d "out" ]; then
		mkdir out
	fi

	cd SFML

	if [ ! -d "Build" ]; then
		mkdir Build
	fi

	cd Build
	echo -e "[SCRIPT] Generating SFML Compiling properties at ${PWD}!"
	
	case $BUILD_TARGET in
		"mingw")
			cmake -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=FALSE -DSFML_USE_STATIC_STD_LIBS=TRUE -DCMAKE_INSTALL_PREFIX=${CURRENT_PATH}/out ..
			;;
	esac

	echo -e "[SCRIPT] Building SFML at "$PWD"!"
	cmake --build .
	cmake --install .
	cd ../..
}



# - - - - - - - - - -
# Confirms the SFMl Current folders
# - - - - - - - - - -
ValidateSFML () {
	if [ ! -d "SFML" ]; then
		echo -e "[SCRIPT] SFML not found"

		git clone $SFML_GIT
		BuildSFML
	fi

	if [ ! -d "SFML/Build" ]; then
		echo -e "[SCRIPT] Build folder not found"
		BuildSFML
	fi
}


# - - - - - - - - - -
# - - - - - - - - - -
PrintHelp () {
	echo -e "______________________________________________________________________"
	echo -e "Build help for the project                                           |" 
	echo -e "_____________________________________________________________________|"
	echo -e "*-r (--run)       | Will run the compilation result                  |"
	echo -e "*-c (--clearall)  | Will clear the previous compilation and rebuild  |"
	echo -e "*-t (--target=)   | Changes the target generator                     |"
	echo -e "   |--> Available: mingw                                             |"
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
			esac
			shift
			;;
		-r|--run)
			RUN_CONFIG=TRUE
			shift
			;;
		-h|--help)
			PrintHelp
			exit 0
	esac
done


if [ "$CLEAR_ALL" = TRUE ]; then
	echo -e "[SCRIPT] Cleaning up"
		rm build -R
fi

ValidateSFML



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
		cmake -G "MinGW Makefiles" ..
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
