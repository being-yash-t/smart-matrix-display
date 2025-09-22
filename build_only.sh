#!/bin/bash

# Build-only script for dB Meter Application (no auto-run)

# Compiler settings
CXX=g++
CXXFLAGS="-O3 -Wall -pthread"
INCLUDES="-I../../include -I."
LIBS="../../lib/librgbmatrix.a -lrt -lm"

# Source files (using the new clean modular structure)
SOURCES="main_clean.cc db_meter_app_new.cpp db_display_new.cpp input_handler.cpp blink_manager.cpp config.cpp arg_parser.cpp"

# Output executable
TARGET="db_meter"

echo "Building dB Meter Application..."
echo "Sources: $SOURCES"

# Compile the application
$CXX $CXXFLAGS $INCLUDES -o $TARGET $SOURCES $LIBS

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful! Executable: $TARGET"
    echo "To run: sudo ./$TARGET"
else
    echo "Build failed!"
    exit 1
fi
