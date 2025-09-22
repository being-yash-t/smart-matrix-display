#!/bin/bash

# Script to transfer fixed files to RPi
# Run this when your RPi is reachable

RPI_IP="192.168.1.87"
RPI_USER="jay_here"
RPI_PATH="/home/jay_here/rpi-rgb-led-matrix/examples-api-use/Stats Boards/"

echo "Transferring fixed files to RPi..."

# Transfer the fixed files
scp db_display.h db_display.cpp db_meter_app.cpp "$RPI_USER@$RPI_IP:$RPI_PATH"

if [ $? -eq 0 ]; then
    echo "Files transferred successfully!"
    echo "You can now run: ./build.sh on your RPi"
else
    echo "Transfer failed. Make sure your RPi is reachable."
fi
