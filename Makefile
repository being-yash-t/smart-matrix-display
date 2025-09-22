# Makefile for LED Matrix Applications

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3 -pthread
INCLUDES = -I../../include -I.
LIBS = ../../lib/librgbmatrix.a -lrt -lm -lpthread

# Source files (organized structure)
CORE_SOURCES = core/config.cpp core/arg_parser.cpp core/input_handler.cpp core/blink_manager.cpp
DISPLAY_SOURCES = display/db_display.cpp
DB_METER_SOURCES = features/db_meter/main.cc features/db_meter/db_meter_app.cpp

ALL_SOURCES = $(CORE_SOURCES) $(DISPLAY_SOURCES) $(DB_METER_SOURCES)
OBJECTS = $(ALL_SOURCES:.cc=.o) $(ALL_SOURCES:.cpp=.o)
TARGET = db_meter

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Compile source files
%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall (optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the application (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  run      - Build and run the application"
	@echo "  debug    - Build with debug symbols"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  uninstall- Remove from /usr/local/bin"
	@echo "  help     - Show this help message"

.PHONY: all clean install uninstall run debug help
