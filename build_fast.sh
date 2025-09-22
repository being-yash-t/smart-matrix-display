#!/bin/bash

# Fast build script for Pi Zero 2 W
# Uses Makefile with parallel compilation

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

# Check for help
if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo -e "${BLUE}${BOLD}Fast Build Script for Pi Zero 2 W${NC}"
    echo -e "${YELLOW}Usage:${NC} $0 [options]"
    echo ""
    echo -e "${BLUE}Options:${NC}"
    echo -e "  ${BOLD}-r${NC}     Build and run immediately"
    echo -e "  ${BOLD}-c${NC}     Clean build artifacts first"
    echo -e "  ${BOLD}-j N${NC}   Use N parallel jobs (default: 2)"
    echo -e "  ${BOLD}-h${NC}     Show this help"
    echo ""
    echo -e "${BLUE}Examples:${NC}"
    echo -e "  $0              # Just build"
    echo -e "  $0 -r           # Build and run"
    echo -e "  $0 -c -r        # Clean, build, and run"
    echo -e "  $0 -j 2         # Use 2 parallel jobs"
    exit 0
fi

# Parse arguments
CLEAN_FIRST=false
RUN_AFTER_BUILD=false
JOBS=2

while [[ $# -gt 0 ]]; do
    case $1 in
        -r|--run)
            RUN_AFTER_BUILD=true
            shift
            ;;
        -c|--clean)
            CLEAN_FIRST=true
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        *)
            echo -e "${RED}${BOLD}Error:${NC} Unknown option: $1"
            echo -e "Use ${YELLOW}-h${NC} or ${YELLOW}--help${NC} for usage information"
            exit 1
            ;;
    esac
done

echo -e "${PURPLE}${BOLD}🚀 Fast Building LED Matrix Applications...${NC}"
echo -e "${BLUE}💡 Optimized for Pi Zero 2 W with ${BOLD}${JOBS}${NC}${BLUE} parallel jobs${NC}"

# Clean if requested
if [ "$CLEAN_FIRST" = true ]; then
    echo -e "${YELLOW}🧹 Cleaning previous build...${NC}"
    make clean
fi

# Build with parallel compilation
echo -e "${YELLOW}⚙️  Compiling with ${JOBS} parallel jobs...${NC}"
make -j${JOBS}

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}${BOLD}✅ Build successful!${NC}"
    echo -e "${GREEN}📦 Executable:${NC} ${BOLD}$TARGET${NC}"
    
    if [ "$RUN_AFTER_BUILD" = true ]; then
        echo ""
        echo -e "${PURPLE}${BOLD}🚀 Running application...${NC}"
        echo -e "${YELLOW}💡 Press Ctrl+C to exit${NC}"
        echo -e "${BLUE}💡 Usage: ${BOLD}./build_fast.sh -r [-b brightness]${NC} ${BLUE}(e.g., -b 8 for 80% brightness)${NC}"
        echo ""
        sudo ./$TARGET "${@:2}"
    else
        echo ""
        echo -e "${BLUE}💡 To run:${NC} ${BOLD}sudo ./$TARGET${NC}"
        echo -e "${BLUE}💡 Or use:${NC} ${BOLD}./build_fast.sh -r${NC} ${BLUE}to build and run${NC}"
        echo -e "${BLUE}💡 Available apps:${NC} ${BOLD}db${NC} ${BLUE}(dB meter),${NC} ${BOLD}youtube${NC} ${BLUE}(subscriber counter),${NC} ${BOLD}spotify${NC} ${BLUE}(artist stats)${NC}"
    fi
else
    echo ""
    echo -e "${RED}${BOLD}❌ Build failed!${NC}"
    echo -e "${RED}💡 Check the error messages above${NC}"
    exit 1
fi
