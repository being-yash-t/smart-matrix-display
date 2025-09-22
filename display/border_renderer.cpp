#include "border_renderer.h"
#include <iostream>

BorderRenderer::BorderRenderer() : borderThickness_(Config::BORDER_THICKNESS) {
}

BorderRenderer::~BorderRenderer() {
}

void BorderRenderer::drawBorder(Canvas* canvas, int dbValue, bool shouldShow) {
    if (!shouldShow) return;
    
    int r, g, b;
    getBorderColor(dbValue, r, g, b);
    
    // Border colors are already inherently dimmer, no additional brightness scaling
    
    int rows = canvas->height();
    int cols = canvas->width();
    
    for (int t = 0; t < borderThickness_; t++) {
        // Top and bottom borders
        for (int x = t; x < cols - t; x++) {
            canvas->SetPixel(x, t, r, g, b);               // Top
            canvas->SetPixel(x, rows - 1 - t, r, g, b);    // Bottom
        }
        // Left and right borders
        for (int y = t; y < rows - t; y++) {
            canvas->SetPixel(t, y, r, g, b);               // Left
            canvas->SetPixel(cols - 1 - t, y, r, g, b);    // Right
        }
    }
}

bool BorderRenderer::shouldBlink(int dbValue) const {
    return dbValue >= Config::YELLOW_THRESHOLD;
}

int BorderRenderer::getBlinkDuration(int dbValue) const {
    if (dbValue >= Config::YELLOW_THRESHOLD && dbValue < Config::ORANGE_THRESHOLD) {
        return Config::BLINK_DURATION_SLOW;    // 80-89dB: slow fade
    } else if (dbValue >= Config::ORANGE_THRESHOLD && dbValue < Config::RED_THRESHOLD) {
        return Config::BLINK_DURATION_MEDIUM;  // 90-94dB: medium flash
    } else if (dbValue >= Config::RED_THRESHOLD) {
        return Config::BLINK_DURATION_FAST;    // 95dB+: fast flash
    }
    return Config::BLINK_DURATION_DEFAULT;
}

void BorderRenderer::setThickness(int thickness) {
    if (thickness > 0) {
        borderThickness_ = thickness;
    }
}

int BorderRenderer::getThickness() const {
    return borderThickness_;
}

void BorderRenderer::getBorderColor(int dbValue, int& r, int& g, int& b) const {
    // Define transition zones (5dB before each threshold)
    const int GREEN_TO_YELLOW_START = Config::YELLOW_THRESHOLD - 5;  // 75dB
    const int YELLOW_TO_ORANGE_START = Config::ORANGE_THRESHOLD - 5; // 85dB  
    const int ORANGE_TO_RED_START = Config::RED_THRESHOLD - 5;       // 90dB
    
    if (dbValue < GREEN_TO_YELLOW_START) {
        // Pure grey (below 75dB)
        r = Config::Colors::BORDER_GREY_R;
        g = Config::Colors::BORDER_GREY_G;
        b = Config::Colors::BORDER_GREY_B;
        
    } else if (dbValue < Config::YELLOW_THRESHOLD) {
        // Transition from grey to yellow (75-79dB)
        float ratio = (float)(dbValue - GREEN_TO_YELLOW_START) / 5.0f;
        blendColors(Config::Colors::BORDER_GREY_R, Config::Colors::BORDER_GREY_G, Config::Colors::BORDER_GREY_B,
                   Config::Colors::BORDER_YELLOW_R, Config::Colors::BORDER_YELLOW_G, Config::Colors::BORDER_YELLOW_B,
                   ratio, r, g, b);
                   
    } else if (dbValue < YELLOW_TO_ORANGE_START) {
        // Pure yellow (80-84dB)
        r = Config::Colors::BORDER_YELLOW_R;
        g = Config::Colors::BORDER_YELLOW_G;
        b = Config::Colors::BORDER_YELLOW_B;
        
    } else if (dbValue < Config::ORANGE_THRESHOLD) {
        // Transition from yellow to orange (85-89dB)
        float ratio = (float)(dbValue - YELLOW_TO_ORANGE_START) / 5.0f;
        blendColors(Config::Colors::BORDER_YELLOW_R, Config::Colors::BORDER_YELLOW_G, Config::Colors::BORDER_YELLOW_B,
                   Config::Colors::BORDER_ORANGE_R, Config::Colors::BORDER_ORANGE_G, Config::Colors::BORDER_ORANGE_B,
                   ratio, r, g, b);
                   
    } else if (dbValue < ORANGE_TO_RED_START) {
        // Pure orange (90-89dB)
        r = Config::Colors::BORDER_ORANGE_R;
        g = Config::Colors::BORDER_ORANGE_G;
        b = Config::Colors::BORDER_ORANGE_B;
        
    } else if (dbValue < Config::RED_THRESHOLD) {
        // Transition from orange to red (90-94dB)
        float ratio = (float)(dbValue - ORANGE_TO_RED_START) / 5.0f;
        blendColors(Config::Colors::BORDER_ORANGE_R, Config::Colors::BORDER_ORANGE_G, Config::Colors::BORDER_ORANGE_B,
                   Config::Colors::BORDER_RED_R, Config::Colors::BORDER_RED_G, Config::Colors::BORDER_RED_B,
                   ratio, r, g, b);
                   
    } else {
        // Pure red (95dB+)
        r = Config::Colors::BORDER_RED_R;
        g = Config::Colors::BORDER_RED_G;
        b = Config::Colors::BORDER_RED_B;
    }
}

void BorderRenderer::blendColors(int r1, int g1, int b1, int r2, int g2, int b2, 
                                float ratio, int& outR, int& outG, int& outB) const {
    // Clamp ratio between 0.0 and 1.0
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    
    // Linear interpolation between two colors
    outR = (int)(r1 + (r2 - r1) * ratio);
    outG = (int)(g1 + (g2 - g1) * ratio);
    outB = (int)(b1 + (b2 - b1) * ratio);
    
    // Clamp values to valid RGB range (0-255)
    if (outR < 0) outR = 0; if (outR > 255) outR = 255;
    if (outG < 0) outG = 0; if (outG > 255) outG = 255;
    if (outB < 0) outB = 0; if (outB > 255) outB = 255;
}
