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
./led_matrix_apps -b 8

# Run with 10% brightness (dim)
./led_matrix_apps -b 1

# Run with 100% brightness (bright)
./led_matrix_apps -b 10

# Show help
./led_matrix_apps -h
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
g++ -O3 -Wall -pthread -I../../include -I. -Isrc -o led_matrix_apps \
    src/application/main.cc \
    src/application/main_app.cpp \
    src/presentation/controllers/db_meter_app.cpp \
    src/presentation/controllers/db_color_calculator.cpp \
    src/presentation/displays/db_display.cpp \
    src/infrastructure/display/border_renderer.cpp \
    src/infrastructure/input/input_handler.cpp \
    src/shared/utils/blink_manager.cpp \
    src/infrastructure/config/config.cpp \
    src/infrastructure/config/arg_parser.cpp \
    src/shared/utils/color_utils.cpp \
    ../../lib/librgbmatrix.a -lrt -lm && sudo ./led_matrix_apps
```

## File Structure

### Clean Architecture Structure
```
src/
├── application/           # Application layer
│   ├── main.cc           # Entry point
│   └── main_app.h/.cpp   # Main orchestrator
│
├── domain/               # Domain layer (business logic)
│   ├── entities/         # Domain entities
│   └── services/         # Domain services
│
├── infrastructure/       # Infrastructure layer
│   ├── config/          # Configuration
│   │   ├── config.h/.cpp
│   │   └── arg_parser.h/.cpp
│   ├── display/         # Low-level display
│   │   └── border_renderer.h/.cpp
│   ├── input/           # Input handling
│   │   └── input_handler.h/.cpp
│   └── network/         # External APIs
│       ├── spotify_api.h/.cpp
│       └── youtube_api.h/.cpp
│
├── presentation/         # Presentation layer
│   ├── controllers/     # Application controllers
│   │   ├── db_meter_app.h/.cpp
│   │   ├── db_color_calculator.h/.cpp
│   │   ├── spotify_app.h/.cpp
│   │   └── youtube_app.h/.cpp
│   └── displays/        # Display rendering
│       ├── db_display.h/.cpp
│       ├── spotify_display.h/.cpp
│       ├── youtube_display.h/.cpp
│       └── text_display.h/.cpp
│
└── shared/              # Shared utilities
    ├── network/         # Network utilities
    │   └── network_handler.h/.cpp
    └── utils/           # Common utilities
        ├── color_utils.h/.cpp
        ├── blink_manager.h/.cpp
        └── rotating_text.h/.cpp
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
