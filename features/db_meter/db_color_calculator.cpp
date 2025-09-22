#include "db_color_calculator.h"

ColorUtils::Color DbColorCalculator::getBorderColor(int dbValue) {
    // Define transition zones (5dB before each threshold)
    const int GREEN_TO_YELLOW_START = YELLOW_THRESHOLD - 5;  // 75dB
    const int YELLOW_TO_ORANGE_START = ORANGE_THRESHOLD - 5; // 85dB  
    const int ORANGE_TO_RED_START = RED_THRESHOLD - 5;       // 90dB
    
    // Define colors
    ColorUtils::Color grey(Config::Colors::BORDER_GREY_R, Config::Colors::BORDER_GREY_G, Config::Colors::BORDER_GREY_B);
    ColorUtils::Color yellow(Config::Colors::BORDER_YELLOW_R, Config::Colors::BORDER_YELLOW_G, Config::Colors::BORDER_YELLOW_B);
    ColorUtils::Color orange(Config::Colors::BORDER_ORANGE_R, Config::Colors::BORDER_ORANGE_G, Config::Colors::BORDER_ORANGE_B);
    ColorUtils::Color red(Config::Colors::BORDER_RED_R, Config::Colors::BORDER_RED_G, Config::Colors::BORDER_RED_B);
    
    if (dbValue < GREEN_TO_YELLOW_START) {
        // Pure grey (below 75dB)
        return grey;
        
    } else if (dbValue < YELLOW_THRESHOLD) {
        // Transition from grey to yellow (75-79dB)
        return getTransitionColor(dbValue, GREEN_TO_YELLOW_START, YELLOW_THRESHOLD, grey, yellow);
        
    } else if (dbValue < YELLOW_TO_ORANGE_START) {
        // Pure yellow (80-84dB)
        return yellow;
        
    } else if (dbValue < ORANGE_THRESHOLD) {
        // Transition from yellow to orange (85-89dB)
        return getTransitionColor(dbValue, YELLOW_TO_ORANGE_START, ORANGE_THRESHOLD, yellow, orange);
        
    } else if (dbValue < ORANGE_TO_RED_START) {
        // Pure orange (90-89dB)
        return orange;
        
    } else if (dbValue < RED_THRESHOLD) {
        // Transition from orange to red (90-94dB)
        return getTransitionColor(dbValue, ORANGE_TO_RED_START, RED_THRESHOLD, orange, red);
        
    } else {
        // Pure red (95dB+)
        return red;
    }
}

ColorUtils::Color DbColorCalculator::getProgressBarColor(int dbValue, int segment) {
    // This would contain the progress bar color logic
    // For now, return basic colors
    switch (segment) {
        case 0: return ColorUtils::Color(Config::Colors::GREEN_R, Config::Colors::GREEN_G, Config::Colors::GREEN_B);
        case 1: return ColorUtils::Color(Config::Colors::YELLOW_R, Config::Colors::YELLOW_G, Config::Colors::YELLOW_B);
        case 2: return ColorUtils::Color(Config::Colors::RED_R, Config::Colors::RED_G, Config::Colors::RED_B);
        default: return ColorUtils::Color(255, 255, 255); // White fallback
    }
}

bool DbColorCalculator::shouldBlink(int dbValue) {
    return dbValue >= YELLOW_THRESHOLD;
}

int DbColorCalculator::getBlinkDuration(int dbValue) {
    if (dbValue >= YELLOW_THRESHOLD && dbValue < ORANGE_THRESHOLD) {
        return Config::BLINK_DURATION_SLOW;    // 80-89dB: slow fade
    } else if (dbValue >= ORANGE_THRESHOLD && dbValue < RED_THRESHOLD) {
        return Config::BLINK_DURATION_MEDIUM;  // 90-94dB: medium flash
    } else if (dbValue >= RED_THRESHOLD) {
        return Config::BLINK_DURATION_FAST;    // 95dB+: fast flash
    }
    return Config::BLINK_DURATION_DEFAULT;
}

ColorUtils::Color DbColorCalculator::getTransitionColor(int dbValue, int startThreshold, int endThreshold,
                                                      const ColorUtils::Color& startColor, 
                                                      const ColorUtils::Color& endColor) {
    float ratio = (float)(dbValue - startThreshold) / (float)(endThreshold - startThreshold);
    
    int outR, outG, outB;
    ColorUtils::blendColors(startColor.r, startColor.g, startColor.b,
                           endColor.r, endColor.g, endColor.b,
                           ratio, outR, outG, outB);
    
    return ColorUtils::Color(outR, outG, outB);
}
