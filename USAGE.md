# dB Meter Application - Usage Guide

## Quick Start

### Build and Run (One Command)
```bash
./build.sh
```
This will compile the application and run it immediately.

### Build Only
```bash
./build_only.sh
```
This will compile the application without running it.

### Run Only (if already built)
```bash
./run.sh
```
This will run the pre-built executable.

## Brightness Control

### Set Brightness Level
```bash
# Run with 80% brightness
./db_meter -b 8

# Run with 10% brightness (dim)
./db_meter -b 1

# Run with 100% brightness (bright)
./db_meter -b 10

# Show help
./db_meter -h
```

### Brightness Levels
- **1** = 10% brightness (very dim)
- **2** = 20% brightness
- **3** = 30% brightness
- **4** = 40% brightness
- **5** = 50% brightness (default)
- **6** = 60% brightness
- **7** = 70% brightness
- **8** = 80% brightness
- **9** = 90% brightness
- **10** = 100% brightness (very bright)

## Manual Compilation

If you prefer to compile manually:
```bash
g++ -O3 -Wall -pthread -I../../include -I. -o db_meter main_clean.cc db_meter_app_new.cpp db_display_new.cpp input_handler.cpp blink_manager.cpp config.cpp arg_parser.cpp ../../lib/librgbmatrix.a -lrt -lm && sudo ./db_meter
```

## File Structure

### Core Components
- **`main_clean.cc`** - Main entry point with argument parsing
- **`db_meter_app_new.cpp/.h`** - Main application class that coordinates everything
- **`db_display_new.cpp/.h`** - Handles all display rendering (text, progress bar, border)
- **`input_handler.cpp/.h`** - Manages non-blocking input from stdin
- **`blink_manager.cpp/.h`** - Manages border blinking states
- **`config.cpp/.h`** - Configuration constants and settings
- **`arg_parser.cpp/.h`** - Command line argument parsing

### Build Scripts
- **`build.sh`** - Build and run in one command
- **`build_only.sh`** - Build only
- **`run.sh`** - Run pre-built executable
- **`Makefile`** - Alternative build system

## Features

- **Modular Design**: Each component is independent and reusable
- **Double Buffering**: No screen flicker during updates
- **Non-blocking Input**: Border blinks continuously while waiting for input
- **Brightness Control**: Adjustable brightness from 10% to 100%
- **Command Line Arguments**: Easy configuration via command line
- **Clean Architecture**: Easy to modify and extend
- **Reduced Border Brightness**: Less intrusive border colors
- **Smaller Font**: More compact display with 4x6 font for units

## Usage

1. Run the application
2. Enter dB values (0-120) and press Enter
3. Watch the display update with:
   - Text showing current dB value
   - Progress bar with color-coded segments
   - Border that blinks based on dB level
4. Press Ctrl+C to exit

## Border Behavior

- **Below 80dB**: Steady light grey border
- **80-89dB**: Yellow border with slow fade
- **90-94dB**: Orange border with medium flash
- **95dB+**: Red border with fast flash
