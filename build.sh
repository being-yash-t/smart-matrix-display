#!/bin/bash

# Build script for dB Meter Application
# Usage: ./build.sh [-r] 
#   -r: Build and run
#   no -r: Build only (default)

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

# Parse command line arguments
RUN_AFTER_BUILD=false
if [ "$1" = "-r" ]; then
    RUN_AFTER_BUILD=true
elif [ "$1" = "--help" ] || [ "$1" = "-h" ]; then
    echo -e "${BOLD}${WHITE}Usage:${NC} $0 [-r]"
    echo -e "  ${GREEN}-r:${NC} Build and run"
    echo -e "  ${BLUE}no -r:${NC} Build only (default)"
    echo -e "  ${YELLOW}-h, --help:${NC} Show this help"
    exit 0
elif [ -n "$1" ]; then
    echo -e "${RED}${BOLD}Error:${NC} Unknown option: $1"
    echo -e "Use ${YELLOW}-h${NC} or ${YELLOW}--help${NC} for usage information"
    exit 1
fi

# Compiler settings
CXX=g++
CXXFLAGS="-O3 -Wall -pthread"
INCLUDES="-I../../include -I."
LIBS="../../lib/librgbmatrix.a -lrt -lm"

# Source files (organized modular structure)
SOURCES="features/db_meter/main.cc features/db_meter/db_meter_app.cpp display/db_display.cpp core/input_handler.cpp core/blink_manager.cpp core/config.cpp core/arg_parser.cpp"

# Output executable
TARGET="db_meter"

echo -e "${BOLD}${CYAN}🔨 Building dB Meter Application...${NC}"
echo -e "${BLUE}📁 Sources:${NC} $SOURCES"
echo ""

# Compile the application
echo -e "${YELLOW}⚙️  Compiling...${NC}"
$CXX $CXXFLAGS $INCLUDES -o $TARGET $SOURCES $LIBS

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}${BOLD}✅ Build successful!${NC}"
    echo -e "${GREEN}📦 Executable:${NC} ${BOLD}$TARGET${NC}"
    
    if [ "$RUN_AFTER_BUILD" = true ]; then
        echo ""
        echo -e "${PURPLE}${BOLD}🚀 Running application...${NC}"
        echo -e "${YELLOW}💡 Press Ctrl+C to exit${NC}"
        echo ""
        sudo ./$TARGET
    else
        echo ""
        echo -e "${BLUE}💡 To run:${NC} ${BOLD}sudo ./$TARGET${NC}"
        echo -e "${BLUE}💡 Or use:${NC} ${BOLD}./build.sh -r${NC} ${BLUE}to build and run${NC}"
    fi
else
    echo ""
    echo -e "${RED}${BOLD}❌ Build failed!${NC}"
    echo -e "${RED}💡 Check the error messages above${NC}"
    exit 1
fi
