#!/usr/bin/env bash
echo -e "[SCRIPT] Regenerating the project"


if [ ! -d "Build" ]; then
  echo -e "[SCRIPT] Build folder not found"
  mkdir Build
fi

cd Build
echo -e "[SCRIPT] Regenerating Project"
cmake ..

echo -e "[SCRIPT] Building Project"
cmake --build .
cd ..