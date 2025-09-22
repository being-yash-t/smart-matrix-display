#include "db_display.h"
#include "presentation/controllers/db_color_calculator.h"
#include <iostream>
#include <cstring>

DbDisplay::DbDisplay(RGBMatrix* matrix, int brightnessLevel) 
    : matrix_(matrix), brightnessLevel_(brightnessLevel), fontsLoaded_(false), borderEnabled_(true) {
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
    
    // Draw border if enabled
    if (borderEnabled_) {
        bool shouldShowBorder = !DbColorCalculator::shouldBlink(dbValue) || blinkState;
        ColorUtils::Color borderColor = DbColorCalculator::getBorderColor(dbValue);
        borderRenderer_.drawBorder(offscreen_, borderColor, shouldShowBorder);
    }
    
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
    int greenEnd = (DbColorCalculator::YELLOW_THRESHOLD * meterWidth) / Config::MAX_DB_VALUE;
    int yellowEnd = (DbColorCalculator::RED_THRESHOLD * meterWidth) / Config::MAX_DB_VALUE;
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


int DbDisplay::getComponentStartY() const {
    int rows = offscreen_->height();
    return (rows - Config::COMPONENT_HEIGHT) / 2;
}


void DbDisplay::enableBorder(bool enable) {
    borderEnabled_ = enable;
}

void DbDisplay::disableBorder() {
    borderEnabled_ = false;
}

bool DbDisplay::isBorderEnabled() const {
    return borderEnabled_;
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
