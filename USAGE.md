# dB Meter Application - Usage Guide

## Quick Start

### Build and Run (One Command)
```bash
./build.sh -r
```
This will compile the application and run it immediately.

### Build Only
```bash
./build.sh
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
g++ -O3 -Wall -pthread -I../../include -I. -o db_meter \
    features/db_meter/main.cc \
    features/db_meter/db_meter_app.cpp \
    features/db_meter/db_color_calculator.cpp \
    display/db_display.cpp \
    display/border_renderer.cpp \
    core/input_handler.cpp \
    core/blink_manager.cpp \
    core/config.cpp \
    core/arg_parser.cpp \
    core/color_utils.cpp \
    ../../lib/librgbmatrix.a -lrt -lm && sudo ./db_meter
```

## File Structure

### Organized Structure
```
├── core/                    # Reusable core components
│   ├── config.h/.cpp       # Generic configuration constants
│   ├── arg_parser.h/.cpp   # Command line parsing
│   ├── input_handler.h/.cpp # Generic input management
│   ├── blink_manager.h/.cpp # Generic blink state management
│   └── color_utils.h/.cpp  # Generic color utilities
│
├── display/                 # Display rendering
│   ├── border_renderer.h/.cpp # Generic border rendering
│   ├── db_display.h/.cpp   # dB meter display
│   └── text_display.h/.cpp # Simple text display
│
└── features/               # Feature applications
    ├── db_meter/          # dB Meter application
    │   ├── main.cc        # Entry point
    │   ├── db_meter_app.h/.cpp # Main application class
    │   └── db_color_calculator.h/.cpp # dB color logic
    └── text_demo/         # Text demo application
        ├── main.cc        # Entry point
        └── text_demo_app.h/.cpp # Main application class
```

### Build Scripts
- **`build.sh`** - Unified build script (use `-r` to run after build)
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
