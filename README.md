# LED Matrix Applications

A modular, reusable framework for creating LED matrix applications with clean architecture and organized code structure.

## 📁 Project Structure (Clean Architecture)

```
src/
├── application/           # Application layer (use cases, main app)
│   ├── main.cc           # Application entry point
│   ├── main_app.h/.cpp   # Main application orchestrator
│
├── domain/               # Domain layer (business logic, entities)
│   ├── entities/         # Domain entities (future expansion)
│   └── services/         # Domain services (future expansion)
│
├── infrastructure/       # Infrastructure layer (external concerns)
│   ├── config/          # Configuration and argument parsing
│   │   ├── config.h/.cpp
│   │   └── arg_parser.h/.cpp
│   ├── display/         # Low-level display components
│   │   └── border_renderer.h/.cpp
│   ├── input/           # Input handling
│   │   └── input_handler.h/.cpp
│   └── network/         # External API integrations
│       ├── spotify_api.h/.cpp
│       └── youtube_api.h/.cpp
│
├── presentation/         # Presentation layer (UI, display logic)
│   ├── controllers/     # Application controllers
│   │   ├── db_meter_app.h/.cpp
│   │   ├── db_color_calculator.h/.cpp
│   │   ├── spotify_app.h/.cpp
│   │   └── youtube_app.h/.cpp
│   └── displays/        # Display rendering components
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

├── build.sh             # Unified build script
├── run.sh               # Run pre-built executable
└── README.md            # This file
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

## 🏗️ Clean Architecture

This project follows Clean Architecture principles with clear separation of concerns:

### Application Layer (`src/application/`)
- **MainApp**: Main application orchestrator that coordinates all components
- **main.cc**: Application entry point

### Domain Layer (`src/domain/`)
- **Entities**: Business objects and domain models (future expansion)
- **Services**: Domain-specific business logic (future expansion)

### Infrastructure Layer (`src/infrastructure/`)
- **Config**: Configuration management and command line argument parsing
- **Display**: Low-level display components and hardware abstraction
- **Input**: Input handling and user interaction
- **Network**: External API integrations (Spotify, YouTube)

### Presentation Layer (`src/presentation/`)
- **Controllers**: Application-specific controllers that orchestrate features
- **Displays**: High-level display rendering components

### Shared Layer (`src/shared/`)
- **Utils**: Common utilities (color blending, blink management, text rotation)
- **Network**: Network utilities and HTTP client functionality

### Key Benefits
- **Separation of Concerns**: Each layer has a specific responsibility
- **Dependency Inversion**: High-level modules don't depend on low-level modules
- **Testability**: Each layer can be tested independently
- **Maintainability**: Changes in one layer don't affect others
- **Extensibility**: Easy to add new features without modifying existing code

## 🔧 Adding New Applications

Following Clean Architecture principles:

1. **Controller**: Create a new controller in `src/presentation/controllers/`
2. **Display**: Create display logic in `src/presentation/displays/`
3. **Infrastructure**: Add any external dependencies in `src/infrastructure/`
4. **Domain**: Add business logic in `src/domain/` (if needed)
5. **Update**: Modify `src/application/main_app.cpp` to include your new controller
6. **Build**: Update `Makefile` to include your new source files

Example for a new Clock application:
```
src/
├── presentation/
│   ├── controllers/
│   │   └── clock_app.h/.cpp      # Clock application controller
│   └── displays/
│       └── clock_display.h/.cpp  # Clock display rendering
├── infrastructure/
│   └── time/                     # Time-related infrastructure
│       └── time_service.h/.cpp
└── domain/
    └── entities/
        └── time_entity.h         # Time domain model
```

## 📋 Requirements

- RGB Matrix Library
- C++11 compatible compiler
- Raspberry Pi with LED matrix hardware

## 🛠️ Manual Compilation

```bash
g++ -O3 -Wall -pthread -I../../include -I. -Isrc \
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
    ../../lib/librgbmatrix.a -lrt -lm \
    -o led_matrix_apps
```

## 📝 Usage

1. Run the application: `./led_matrix_apps -b 8`
2. Use the interactive menu to switch between applications:
   - dB Meter: Audio level monitoring
   - Spotify Counter: Artist statistics display
   - YouTube Counter: Channel statistics display
3. Follow on-screen instructions for each application
4. Press Ctrl+C to exit

## 🎯 Design Principles

- **Modularity**: Each component has a single responsibility
- **Reusability**: Core components can be used across applications
- **Clean Code**: Readable, maintainable, and well-documented
- **Extensibility**: Easy to add new features and applications
- **Performance**: Optimized for smooth LED matrix rendering
