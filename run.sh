#!/bin/bash

# Run script for dB Meter Application

TARGET="db_meter"

# Check if executable exists
if [ ! -f "$TARGET" ]; then
    echo "Executable $TARGET not found!"
    echo "Please run ./build_only.sh first to build the application."
    exit 1
fi

echo "Running dB Meter Application..."
sudo ./$TARGET
