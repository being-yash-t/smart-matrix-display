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
    // Default grey (below 80dB)
    r = Config::Colors::BORDER_GREY_R;
    g = Config::Colors::BORDER_GREY_G;
    b = Config::Colors::BORDER_GREY_B;
    
    if (dbValue >= Config::YELLOW_THRESHOLD && dbValue < Config::ORANGE_THRESHOLD) {
        // Yellow (80-89dB)
        r = Config::Colors::BORDER_YELLOW_R;
        g = Config::Colors::BORDER_YELLOW_G;
        b = Config::Colors::BORDER_YELLOW_B;
    } else if (dbValue >= Config::ORANGE_THRESHOLD && dbValue < Config::RED_THRESHOLD) {
        // Orange (90-94dB)
        r = Config::Colors::BORDER_ORANGE_R;
        g = Config::Colors::BORDER_ORANGE_G;
        b = Config::Colors::BORDER_ORANGE_B;
    } else if (dbValue >= Config::RED_THRESHOLD) {
        // Red (95dB+)
        r = Config::Colors::BORDER_RED_R;
        g = Config::Colors::BORDER_RED_G;
        b = Config::Colors::BORDER_RED_B;
    }
}
