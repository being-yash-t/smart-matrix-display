#include "arg_parser.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

ArgParser::ArgParser(int argc, char* argv[]) 
    : brightness_(Config::DEFAULT_BRIGHTNESS), showHelp_(false) {
    parseArguments(argc, argv);
}

void ArgParser::parseArguments(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--brightness") == 0) {
            if (i + 1 < argc) {
                int brightness = std::atoi(argv[i + 1]);
                if (isValidBrightness(brightness)) {
                    brightness_ = brightness;
                    i++; // Skip the next argument as it's the brightness value
                } else {
                    std::cerr << "Invalid brightness value: " << argv[i + 1] 
                              << ". Must be between " << Config::MIN_BRIGHTNESS 
                              << " and " << Config::MAX_BRIGHTNESS << std::endl;
                }
            } else {
                std::cerr << "Missing brightness value after -b" << std::endl;
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            showHelp_ = true;
        } else {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
        }
    }
}

bool ArgParser::isValidBrightness(int brightness) const {
    return brightness >= Config::MIN_BRIGHTNESS && brightness <= Config::MAX_BRIGHTNESS;
}

void ArgParser::printHelp(const char* programName) const {
    std::cout << "dB Meter Application - Audio Level Monitor\n\n";
    std::cout << "Usage: " << programName << " [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -b, --brightness <1-10>  Set brightness level (1=10%, 10=100%)\n";
    std::cout << "                           Default: " << Config::DEFAULT_BRIGHTNESS << " (50%)\n";
    std::cout << "  -h, --help              Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << programName << "              # Run with default brightness (50%)\n";
    std::cout << "  " << programName << " -b 8         # Run with 80% brightness\n";
    std::cout << "  " << programName << " -b 1         # Run with 10% brightness (dim)\n";
    std::cout << "  " << programName << " -b 10        # Run with 100% brightness (bright)\n\n";
    std::cout << "Controls:\n";
    std::cout << "  Enter dB values (0-120) and press Enter to update display\n";
    std::cout << "  Press Ctrl+C to exit\n\n";
    std::cout << "Display Features:\n";
    std::cout << "  - Text shows current dB value\n";
    std::cout << "  - Progress bar with color-coded segments\n";
    std::cout << "  - Border blinks based on dB level:\n";
    std::cout << "    * Below 80dB: Steady grey border\n";
    std::cout << "    * 80-89dB: Yellow border, slow blink\n";
    std::cout << "    * 90-94dB: Orange border, medium blink\n";
    std::cout << "    * 95dB+: Red border, fast blink\n";
}
