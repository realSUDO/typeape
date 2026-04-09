#!/bin/bash
set -e
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
echo "Building..."
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
cd "$ROOT"
echo "Installing..."
sudo cp build/typeape /usr/local/bin/typeape
sudo mkdir -p /usr/local/share/typeape/data
sudo cp data/words.txt /usr/local/share/typeape/data/
sudo cp data/typeape.desktop /usr/share/applications/ 2>/dev/null || true
echo "TypeApe installed! Run: typeape"
