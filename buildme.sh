#!/usr/bin/env bash
echo -e "[SCRIPT] Regenerating the project"

if [ ! -d "Build" ]; then
	echo -e "[SCRIPT] Build folder not found"
	mkdir Build
fi

echo -e "[SCRIPT] Building SFML Module!"
if [ ! -d "Include" ]; then
	cd SFML
	mkdir Build && cd Build
	echo -e "[SCRIPT] Generating SFML Compiling properties!"
	cmake ..	
	cmake -DBUILD_SHARED_LIBS=FALSE ..
	cmake --build .
	cd ../..

cd Build
echo -e "[SCRIPT] Generating compilation Project"
cmake ..

echo -e "[SCRIPT] Building Project"
cmake --build .
cd ..