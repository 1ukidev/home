#!/usr/bin/env bash

set -e

BUILD_FOLDER="build"
BUILD_TYPE="Release"

programs=(cmake ninja)
for prog in "${programs[@]}"; do
    if ! command -v "$prog" >/dev/null 2>&1; then
        echo "Erro: $prog não encontrado"
        exit 1
    fi
done

if [ ! -d "$BUILD_FOLDER" ]; then
    mkdir "$BUILD_FOLDER"
fi

cd "$BUILD_FOLDER"

echo "CMake:"
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -G Ninja

echo
echo "Ninja:"
ninja -j$(nproc)

echo
echo "Construído com sucesso na pasta $BUILD_FOLDER"
