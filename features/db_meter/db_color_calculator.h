#ifndef DB_COLOR_CALCULATOR_H
#define DB_COLOR_CALCULATOR_H

#include "../../core/color_utils.h"
#include "../../core/config.h"

class DbColorCalculator {
public:
    // dB-specific constants
    static const int YELLOW_THRESHOLD = 80;
    static const int ORANGE_THRESHOLD = 90;
    static const int RED_THRESHOLD = 95;
    
    // Calculate border color based on dB value
    static ColorUtils::Color getBorderColor(int dbValue);
    
    // Calculate progress bar colors based on dB value
    static ColorUtils::Color getProgressBarColor(int dbValue, int segment);
    
    // Check if border should blink
    static bool shouldBlink(int dbValue);
    
    // Get blink duration based on dB value
    static int getBlinkDuration(int dbValue);
    
private:
    // Helper methods for color transitions
    static ColorUtils::Color getTransitionColor(int dbValue, int startThreshold, int endThreshold,
                                               const ColorUtils::Color& startColor, 
                                               const ColorUtils::Color& endColor);
};

#endif // DB_COLOR_CALCULATOR_H
