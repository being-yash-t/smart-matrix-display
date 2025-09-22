# Makefile for LED Matrix Applications
# Optimized for Pi Zero 2 W

# Compiler settings
CXX = g++
CXXFLAGS = -O2 -Wall -pthread -std=c++11
INCLUDES = -I../../include -I.
LIBS = ../../lib/librgbmatrix.a -lrt -lm -lcurl

# Target executable
TARGET = led_matrix_apps

# Source files
SOURCES = main.cc main_app.cpp \
          features/db_meter/db_meter_app.cpp \
          features/db_meter/db_color_calculator.cpp \
          features/youtube_counter/youtube_app.cpp \
          features/youtube_counter/network/youtube_api.cpp \
          display/db_display.cpp \
          display/youtube_display.cpp \
          display/border_renderer.cpp \
          core/input_handler.cpp \
          core/blink_manager.cpp \
          core/config.cpp \
          core/arg_parser.cpp \
          core/color_utils.cpp \
          core/rotating_text.cpp \
          core/network_handler.cpp

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