#include "color_utils.h"

void ColorUtils::blendColors(int r1, int g1, int b1, int r2, int g2, int b2, 
                           float ratio, int& outR, int& outG, int& outB) {
    // Clamp ratio between 0.0 and 1.0
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    
    // Linear interpolation between two colors
    outR = (int)(r1 + (r2 - r1) * ratio);
    outG = (int)(g1 + (g2 - g1) * ratio);
    outB = (int)(b1 + (b2 - b1) * ratio);
    
    // Clamp values to valid RGB range (0-255)
    clampColor(outR, outG, outB);
}

void ColorUtils::clampColor(int& r, int& g, int& b) {
    if (r < 0) r = 0; 
    if (r > 255) r = 255;
    if (g < 0) g = 0; 
    if (g > 255) g = 255;
    if (b < 0) b = 0; 
    if (b > 255) b = 255;
}
