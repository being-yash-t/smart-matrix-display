#!/bin/bash

# Run script for dB Meter Application

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
BOLD='\033[1m'
NC='\033[0m' # No Color

TARGET="led_matrix_apps"

# Check if executable exists
if [ ! -f "$TARGET" ]; then
    echo -e "${RED}${BOLD}❌ Executable $TARGET not found!${NC}"
    echo -e "${YELLOW}💡 Please run ${BOLD}./build.sh${NC} ${YELLOW}first to build the application.${NC}"
    echo -e "${BLUE}💡 Or use ${BOLD}./build.sh -r${NC} ${BLUE}to build and run in one command.${NC}"
    exit 1
fi

echo -e "${PURPLE}${BOLD}🚀 Running LED Matrix Applications...${NC}"
echo -e "${YELLOW}💡 Press Ctrl+C to exit${NC}"
echo -e "${BLUE}💡 Usage: ${BOLD}./run.sh [-b brightness]${NC} ${BLUE}(e.g., -b 8 for 80% brightness)${NC}"
echo ""
sudo ./$TARGET "$@"
