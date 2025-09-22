# Makefile for LED Matrix Applications
# Optimized for Pi Zero 2 W

# Compiler settings
CXX = g++
CXXFLAGS = -O2 -Wall -pthread -std=c++11
INCLUDES = -I../../include -I. -Isrc
LIBS = ../../lib/librgbmatrix.a -lrt -lm -lcurl

# Target executable
TARGET = led_matrix_apps

# Source files
SOURCES = src/application/main.cc \
          src/application/main_app.cpp \
          src/presentation/controllers/db_meter_app.cpp \
          src/presentation/controllers/db_color_calculator.cpp \
          src/presentation/controllers/youtube_app.cpp \
          src/infrastructure/network/youtube_api.cpp \
          src/presentation/controllers/spotify_app.cpp \
          src/infrastructure/network/spotify_api.cpp \
          src/presentation/displays/db_display.cpp \
          src/presentation/displays/youtube_display.cpp \
          src/presentation/displays/spotify_display.cpp \
          src/presentation/displays/text_display.cpp \
          src/infrastructure/display/border_renderer.cpp \
          src/infrastructure/input/input_handler.cpp \
          src/shared/utils/blink_manager.cpp \
          src/infrastructure/config/config.cpp \
          src/infrastructure/config/arg_parser.cpp \
          src/shared/utils/color_utils.cpp \
          src/shared/utils/rotating_text.cpp \
          src/shared/network/network_handler.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.cc=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	@echo "🔗 Linking..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

# Compile source files to object files
%.o: %.cpp
	@echo "⚙️  Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cc
	@echo "⚙️  Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	@echo "🧹 Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET)

# Install dependencies (if needed)
install-deps:
	@echo "📦 Installing dependencies..."
	sudo apt-get update
	sudo apt-get install -y build-essential

# Run the application
run: $(TARGET)
	@echo "🚀 Running application..."
	sudo ./$(TARGET)

# Build and run
build-run: $(TARGET)
	@echo "🚀 Running application..."
	sudo ./$(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all        - Build the application (default)"
	@echo "  clean      - Remove build artifacts"
	@echo "  run        - Build and run the application"
	@echo "  build-run  - Build and run the application"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "For parallel compilation, use: make -j4"

.PHONY: all clean install-deps run build-run help