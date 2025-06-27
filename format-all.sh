#!/bin/bash

# Цвета ANSI
CYAN='\033[0;36m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

SOURCE_DIR=$(pwd)
EXTENSIONS=("*.cpp" "*.hpp" "*.h" "*.cc" "*.cxx" "*.c" "*.hh")

for ext in "${EXTENSIONS[@]}"; do
  find "$SOURCE_DIR" -type d -name "vcpkg" -prune -o -type f -name "$ext" -print | while read -r file; do
    filename=$(basename "$file")
    echo -e "${CYAN}[clang-format]${NC}: formatting ${YELLOW}$filename${NC}"
    clang-format -i "$file"
  done
done

echo -e "${CYAN}[clang-format]: ${GREEN}✅ All files formatted successfully"
