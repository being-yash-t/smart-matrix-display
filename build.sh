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

# Compiler settings optimized for Pi Zero 2 W
CXX=g++
CXXFLAGS="-O2 -Wall -pthread"  # -O2 instead of -O3 for faster compilation
INCLUDES="-I../../include -I. -Isrc"
LIBS="../../lib/librgbmatrix.a -lrt -lm -lcurl"

# Source files (clean architecture structure)
SOURCES="src/application/main.cc src/application/main_app.cpp src/presentation/controllers/db_meter_app.cpp src/presentation/controllers/db_color_calculator.cpp src/presentation/controllers/youtube_app.cpp src/infrastructure/network/youtube_api.cpp src/presentation/controllers/spotify_app.cpp src/infrastructure/network/spotify_api.cpp src/presentation/displays/db_display.cpp src/presentation/displays/youtube_display.cpp src/presentation/displays/spotify_display.cpp src/presentation/displays/text_display.cpp src/infrastructure/display/border_renderer.cpp src/infrastructure/input/input_handler.cpp src/shared/utils/blink_manager.cpp src/infrastructure/config/config.cpp src/infrastructure/config/arg_parser.cpp src/shared/utils/color_utils.cpp src/shared/utils/rotating_text.cpp src/shared/network/network_handler.cpp"

# Output executable
TARGET="led_matrix_apps"

echo -e "${BOLD}${CYAN}🔨 Building LED Matrix Applications...${NC}"
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
        echo -e "${BLUE}💡 Usage: ${BOLD}./build.sh -r [-b brightness]${NC} ${BLUE}(e.g., -b 8 for 80% brightness)${NC}"
        echo ""
        sudo -E ./$TARGET "${@:2}"
    else
        echo ""
        echo -e "${BLUE}💡 To run:${NC} ${BOLD}sudo ./$TARGET${NC}"
        echo -e "${BLUE}💡 Or use:${NC} ${BOLD}./build.sh -r${NC} ${BLUE}to build and run${NC}"
        echo -e "${BLUE}💡 Available apps:${NC} ${BOLD}db${NC} ${BLUE}(dB meter),${NC} ${BOLD}youtube${NC} ${BLUE}(subscriber counter),${NC} ${BOLD}spotify${NC} ${BLUE}(artist stats)${NC}"
    fi
else
    echo ""
    echo -e "${RED}${BOLD}❌ Build failed!${NC}"
    echo -e "${RED}💡 Check the error messages above${NC}"
    exit 1
fi
