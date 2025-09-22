# LED Matrix Applications

A modular, reusable framework for creating LED matrix applications with clean architecture and organized code structure.

## 📁 Project Structure

```
├── core/                    # Core reusable components
│   ├── config.h/.cpp       # Configuration constants and settings
│   ├── arg_parser.h/.cpp   # Command line argument parsing
│   ├── input_handler.h/.cpp # Non-blocking input management
│   └── blink_manager.h/.cpp # Blink state management
│
├── display/                 # Display rendering components
│   ├── db_display.h/.cpp   # Display rendering engine
│
├── features/               # Feature-specific applications
│   └── db_meter/          # dB Level Meter Application
│       ├── main.cc        # Application entry point
│       ├── db_meter_app.h/.cpp # Main application class
│
├── build.sh               # Unified build script
├── run.sh                 # Run pre-built executable
└── README.md              # This file
```

## 🚀 Quick Start

### Build and Run dB Meter
```bash
./build.sh -r
```

### Build Only
```bash
./build.sh
```

### Run Pre-built Executable
```bash
./run.sh
```

## 🎛️ dB Meter Features

### Brightness Control
```bash
./db_meter -b 8    # 80% brightness
./db_meter -b 1    # 10% brightness (dim)
./db_meter -b 10   # 100% brightness (bright)
./db_meter -h      # Show help
```

### Display Features
- **Text**: Shows current dB value with large number and small "dB" unit
- **Progress Bar**: Color-coded segments (green → yellow → red)
- **Border**: Blinks based on dB level with inherently dimmer colors
- **Double Buffering**: Smooth, flicker-free updates

### Border Behavior
- **Below 80dB**: Steady grey border
- **80-89dB**: Yellow border, slow blink
- **90-94dB**: Orange border, medium blink
- **95dB+**: Red border, fast blink

## 🏗️ Architecture

### Core Components
- **Config**: Centralized configuration and constants
- **ArgParser**: Command line argument handling
- **InputHandler**: Non-blocking stdin input
- **BlinkManager**: Blink state management

### Display Components
- **DbDisplay**: Rendering engine with brightness control
- **Modular Design**: Easy to extend for new display types

### Feature Applications
- **DbMeter**: Audio level monitoring application
- **Extensible**: Easy to add new applications

## 🔧 Adding New Applications

1. Create a new folder in `features/`
2. Add your main application files
3. Update build scripts to include your sources
4. Use core components for common functionality

Example:
```
features/
├── db_meter/          # Existing dB meter
├── clock/             # New clock application
├── weather/           # New weather display
└── custom_app/        # Your new application
```

## 📋 Requirements

- RGB Matrix Library
- C++11 compatible compiler
- Raspberry Pi with LED matrix hardware

## 🛠️ Manual Compilation

```bash
g++ -O3 -Wall -pthread -I../../include -I. \
    features/db_meter/main.cc \
    features/db_meter/db_meter_app_new.cpp \
    display/db_display.cpp \
    core/input_handler.cpp \
    core/blink_manager.cpp \
    core/config.cpp \
    core/arg_parser.cpp \
    ../../lib/librgbmatrix.a -lrt -lm \
    -o db_meter
```

## 📝 Usage

1. Run the application: `./db_meter -b 8`
2. Enter dB values (0-120) and press Enter
3. Watch the display update with visual feedback
4. Press Ctrl+C to exit

## 🎯 Design Principles

- **Modularity**: Each component has a single responsibility
- **Reusability**: Core components can be used across applications
- **Clean Code**: Readable, maintainable, and well-documented
- **Extensibility**: Easy to add new features and applications
- **Performance**: Optimized for smooth LED matrix rendering
