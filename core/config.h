#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
public:
    // Default values
    static const int DEFAULT_BRIGHTNESS = 5;  // 50% brightness
    static const int MIN_BRIGHTNESS = 1;      // 10% brightness
    static const int MAX_BRIGHTNESS = 10;     // 100% brightness
    static const int DEFAULT_DB_VALUE = 60;
    static const int MIN_DB_VALUE = 0;
    static const int MAX_DB_VALUE = 120;
    
    // Display constants
    static const int COMPONENT_HEIGHT = 21;   // text height + spacing + progress bar height
    static const int TEXT_SPACING = 4;        // spacing between text and progress bar
    static const int PROGRESS_BAR_HEIGHT = 2; // height of progress bar
    static const int BORDER_THICKNESS = 4;    // border thickness
    static const int PADDING = 4;             // padding from edges
    
    // Font paths
    static const std::string LARGE_FONT_PATH;
    static const std::string SMALL_FONT_PATH;
    
    // Color definitions
    struct Colors {
        // Text colors
        static const int TEXT_R = 255;
        static const int TEXT_G = 255;
        static const int TEXT_B = 255;
        
        // Progress bar colors
        static const int GREEN_R = 0;
        static const int GREEN_G = 255;
        static const int GREEN_B = 0;
        
        static const int YELLOW_R = 255;
        static const int YELLOW_G = 255;
        static const int YELLOW_B = 0;
        
        static const int RED_R = 255;
        static const int RED_G = 0;
        static const int RED_B = 0;
        
        // Border colors (inherently dimmer, not affected by main brightness)
        static const int BORDER_GREY_R = 100;    // Even more reduced
        static const int BORDER_GREY_G = 100;    // Even more reduced
        static const int BORDER_GREY_B = 100;    // Even more reduced
        
        static const int BORDER_YELLOW_R = 150;  // Even more reduced
        static const int BORDER_YELLOW_G = 150;  // Even more reduced
        static const int BORDER_YELLOW_B = 0;
        
        static const int BORDER_ORANGE_R = 180;  // Slightly reduced
        static const int BORDER_ORANGE_G = 80;   // More reduced
        static const int BORDER_ORANGE_B = 0;
        
        static const int BORDER_RED_R = 255;
        static const int BORDER_RED_G = 0;
        static const int BORDER_RED_B = 0;
    };
    
    // dB thresholds
    static const int YELLOW_THRESHOLD = 80;
    static const int ORANGE_THRESHOLD = 90;
    static const int RED_THRESHOLD = 95;
    
    // Blink durations (microseconds)
    static const int BLINK_DURATION_DEFAULT = 200000;
    static const int BLINK_DURATION_SLOW = 500000;    // 80-89dB
    static const int BLINK_DURATION_MEDIUM = 250000;  // 90-94dB
    static const int BLINK_DURATION_FAST = 100000;    // 95dB+
};

#endif // CONFIG_H
