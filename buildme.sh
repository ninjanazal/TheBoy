#!/usr/bin/env bash

# - - - - - - - - - -
# Holds the base global path
# - - - - - - - - - -
export CURRENT_PATH=$PWD



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
	echo -e "[SCRIPT] Generating SFML Compiling properties at "$PWD"!"
	cmake -DBUILD_SHARED_LIBS=FALSE -DSFML_USE_STATIC_STD_LIBS=TRUE -DCMAKE_INSTALL_PREFIX=$CURRENT_PATH/out ..

	echo -e "[SCRIPT] Building SFML at "$PWD"!"
	cmake . --build 
	#cmake . --install
	cd ../..
}


echo -e "[SCRIPT] Regenerating the project"


# - - - - - - - - - -
# Parameters and current project layout
# - - - - - - - - - -
if [ ! $# -lt 1 ]; then
	if [ $1 = "all" ]; then
		echo -e "[SCRIPT] Building SFML"
		BuildSFML
	fi
elif [ ! -d "SFML/Build" ]; then
	echo -e "[SCRIPT] Build folder not found"
	BuildSFML
fi


# - - - - - - - - - -
# Base Project Generation & Compilation
# - - - - - - - - - -
echo -e "[SCRIPT] Moving to TheBoy Files at "$PWD"!"
cd Build
echo -e "[SCRIPT] Generating compilation Project at "$PWD""
cmake ..

echo -e "[SCRIPT] Building Project"
cmake --build .
cd ..