#include "db_display.h"
#include <iostream>
#include <cstring>

DbDisplay::DbDisplay(RGBMatrix* matrix, int brightnessLevel) 
    : matrix_(matrix), brightnessLevel_(brightnessLevel), fontsLoaded_(false) {
    offscreen_ = matrix_->CreateFrameCanvas();
    brightnessScale_ = (brightnessLevel * 255) / Config::MAX_BRIGHTNESS;
    loadFonts();
}

DbDisplay::~DbDisplay() {
    // FrameCanvas is managed by the matrix, no need to delete
}

void DbDisplay::update(int dbValue, bool blinkState) {
    int componentStartY = getComponentStartY();
    
    // Clear and redraw everything
    clearAndRedraw(dbValue, componentStartY);
    
    // Draw border if needed
    bool shouldShowBorder = !shouldBlink(dbValue) || blinkState;
    drawBorder(dbValue, shouldShowBorder);
    
    // Swap the offscreen canvas with the visible one (double buffering)
    offscreen_ = matrix_->SwapOnVSync(offscreen_);
}

void DbDisplay::clearAndRedraw(int dbValue, int componentStartY) {
    offscreen_->Clear();
    drawText(dbValue, componentStartY);
    drawProgressBar(dbValue, componentStartY);
}

void DbDisplay::drawText(int dbValue, int componentStartY) {
    if (!fontsLoaded_) return;
    
    // Apply brightness scaling to text colors
    int textR = scaleBrightness(Config::Colors::TEXT_R);
    int textG = scaleBrightness(Config::Colors::TEXT_G);
    int textB = scaleBrightness(Config::Colors::TEXT_B);
    rgb_matrix::Color white(textR, textG, textB);
    
    // Position text to align with progress bar (left-aligned)
    int textX = Config::BORDER_THICKNESS + Config::PADDING;
    
    // Draw large dB number
    char dbBuf[8];
    snprintf(dbBuf, sizeof(dbBuf), "%d", dbValue);
    rgb_matrix::DrawText(offscreen_, largeFont_, textX, componentStartY + largeFont_.height(), white, dbBuf);
    
    // Draw small "dB" unit right after the number
    int unitX = textX + largeFont_.CharacterWidth('0') * strlen(dbBuf) + 2; // 2px spacing
    rgb_matrix::DrawText(offscreen_, smallFont_, unitX, componentStartY + largeFont_.height(), white, "dB");
}

void DbDisplay::drawProgressBar(int dbValue, int componentStartY) {
    int startY = componentStartY + 15 + Config::TEXT_SPACING; // below text with spacing
    int startX = Config::BORDER_THICKNESS + Config::PADDING;
    int meterWidth = offscreen_->width() - 2 * Config::BORDER_THICKNESS - 2 * Config::PADDING;
    
    // Calculate segment positions (assuming 120dB max)
    int greenEnd = (Config::YELLOW_THRESHOLD * meterWidth) / Config::MAX_DB_VALUE;
    int yellowEnd = (Config::RED_THRESHOLD * meterWidth) / Config::MAX_DB_VALUE;
    int totalFill = (dbValue * meterWidth) / Config::MAX_DB_VALUE;
    
    // Draw green segment (0-80dB)
    if (totalFill > 0) {
        int greenFill = (totalFill > greenEnd) ? greenEnd : totalFill;
        int greenR = scaleBrightness(Config::Colors::GREEN_R);
        int greenG = scaleBrightness(Config::Colors::GREEN_G);
        int greenB = scaleBrightness(Config::Colors::GREEN_B);
        drawBarSegment(startX, startY, greenFill, greenR, greenG, greenB);
    }
    
    // Draw yellow segment (80-95dB)
    if (totalFill > greenEnd) {
        int yellowFill = (totalFill > yellowEnd) ? yellowEnd : totalFill;
        int yellowR = scaleBrightness(Config::Colors::YELLOW_R);
        int yellowG = scaleBrightness(Config::Colors::YELLOW_G);
        int yellowB = scaleBrightness(Config::Colors::YELLOW_B);
        drawBarSegment(startX + greenEnd, startY, yellowFill - greenEnd, yellowR, yellowG, yellowB);
    }
    
    // Draw red segment (95dB+)
    if (totalFill > yellowEnd) {
        int redR = scaleBrightness(Config::Colors::RED_R);
        int redG = scaleBrightness(Config::Colors::RED_G);
        int redB = scaleBrightness(Config::Colors::RED_B);
        drawBarSegment(startX + yellowEnd, startY, totalFill - yellowEnd, redR, redG, redB);
    }
}

void DbDisplay::drawBarSegment(int startX, int startY, int width, int r, int g, int b) {
    for (int x = startX; x < startX + width; x++) {
        for (int h = 0; h < Config::PROGRESS_BAR_HEIGHT; h++) {
            offscreen_->SetPixel(x, startY + h, r, g, b);
        }
    }
}

void DbDisplay::drawBorder(int dbValue, bool shouldShow) {
    if (!shouldShow) return;
    
    int r, g, b;
    getBorderColor(dbValue, r, g, b);
    
    // Border colors are already inherently dimmer, no additional brightness scaling
    
    int rows = offscreen_->height();
    int cols = offscreen_->width();
    
    for (int t = 0; t < Config::BORDER_THICKNESS; t++) {
        // Top and bottom borders
        for (int x = t; x < cols - t; x++) {
            offscreen_->SetPixel(x, t, r, g, b);               // Top
            offscreen_->SetPixel(x, rows - 1 - t, r, g, b);    // Bottom
        }
        // Left and right borders
        for (int y = t; y < rows - t; y++) {
            offscreen_->SetPixel(t, y, r, g, b);               // Left
            offscreen_->SetPixel(cols - 1 - t, y, r, g, b);    // Right
        }
    }
}

int DbDisplay::getComponentStartY() const {
    int rows = offscreen_->height();
    return (rows - Config::COMPONENT_HEIGHT) / 2;
}

bool DbDisplay::shouldBlink(int dbValue) const {
    return dbValue >= Config::YELLOW_THRESHOLD;
}

int DbDisplay::getBlinkDuration(int dbValue) const {
    if (dbValue >= Config::YELLOW_THRESHOLD && dbValue < Config::ORANGE_THRESHOLD) {
        return Config::BLINK_DURATION_SLOW;    // 80-89dB: slow fade
    } else if (dbValue >= Config::ORANGE_THRESHOLD && dbValue < Config::RED_THRESHOLD) {
        return Config::BLINK_DURATION_MEDIUM;  // 90-94dB: medium flash
    } else if (dbValue >= Config::RED_THRESHOLD) {
        return Config::BLINK_DURATION_FAST;    // 95dB+: fast flash
    }
    return Config::BLINK_DURATION_DEFAULT;
}

void DbDisplay::getBorderColor(int dbValue, int& r, int& g, int& b) const {
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

int DbDisplay::scaleBrightness(int color) const {
    return (color * brightnessScale_) / 255;
}

void DbDisplay::setBrightness(int brightnessLevel) {
    if (brightnessLevel >= Config::MIN_BRIGHTNESS && brightnessLevel <= Config::MAX_BRIGHTNESS) {
        brightnessLevel_ = brightnessLevel;
        brightnessScale_ = (brightnessLevel * 255) / Config::MAX_BRIGHTNESS;
    }
}

void DbDisplay::loadFonts() {
    fontsLoaded_ = false;
    
    if (!largeFont_.LoadFont(Config::LARGE_FONT_PATH.c_str())) {
        std::cerr << "Large font not found: " << Config::LARGE_FONT_PATH << std::endl;
        return;
    }
    
    if (!smallFont_.LoadFont(Config::SMALL_FONT_PATH.c_str())) {
        std::cerr << "Small font not found: " << Config::SMALL_FONT_PATH << ", using large font" << std::endl;
        smallFont_ = largeFont_;
    }
    
    fontsLoaded_ = true;
}
