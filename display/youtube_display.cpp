#include "youtube_display.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

YoutubeDisplay::YoutubeDisplay(RGBMatrix* matrix, int brightnessLevel) 
    : matrix_(matrix), brightnessLevel_(brightnessLevel), fontsLoaded_(false) {
    offscreen_ = matrix_->CreateFrameCanvas();
    brightnessScale_ = (brightnessLevel * 255) / Config::MAX_BRIGHTNESS;
    loadFonts();
}

YoutubeDisplay::~YoutubeDisplay() {
    // FrameCanvas is managed by the matrix, no need to delete
}

void YoutubeDisplay::update(int subscriberCount) {
    // Clear and redraw everything
    clearAndRedraw(subscriberCount);
    
    // Swap the offscreen canvas with the visible one (double buffering)
    offscreen_ = matrix_->SwapOnVSync(offscreen_);
}

void YoutubeDisplay::clearAndRedraw(int subscriberCount) {
    offscreen_->Clear();
    
    int centerX = offscreen_->width() / 2;
    int screenHeight = offscreen_->height();
    
    // Calculate total height of the unit (icon + gap + text)
    int iconHeight = 13; // YouTube icon height (increased for triangle tip)
    int gap = 4; // 4 pixel gap
    int textHeight = largeFont_.height(); // Text height
    int totalUnitHeight = iconHeight + gap + textHeight;
    
    // Center the entire unit vertically
    int unitStartY = (screenHeight - totalUnitHeight) / 2;
    
    // Draw YouTube icon
    int iconWidth = 18; // YouTube icon width
    int iconStartX = centerX - iconWidth / 2;
    int iconY = unitStartY;
    drawPlayButton(iconStartX, iconY);
    
    // Draw subscriber count below with 4px gap
    int textY = iconY + iconHeight + gap;
    drawSubscriberCount(subscriberCount, centerX, textY);
}

void YoutubeDisplay::drawPlayButton(int startX, int startY) {
    // Apply brightness scaling to colors
    int redR = scaleBrightness(255);   // YouTube red
    int redG = scaleBrightness(0);
    int redB = scaleBrightness(0);
    
    int whiteR = scaleBrightness(255); // White triangle
    int whiteG = scaleBrightness(255);
    int whiteB = scaleBrightness(255);
    
    // Draw red rectangular background
    int rectWidth = 18;
    int rectHeight = 13;  // Increased height for triangle tip visibility
    int centerY = startY + rectHeight / 2;
    
    // Calculate center for rectangle
    int centerX = startX + rectWidth / 2;
    
    // Draw rectangular background
    for (int y = startY; y < startY + rectHeight; y++) {
        for (int x = startX; x < startX + rectWidth; x++) {
            offscreen_->SetPixel(x, y, redR, redG, redB);
        }
    }
    
    // Draw white triangle (play button) - simple right-pointing triangle
    int triangleHeight = 7;  // Increased height for better visibility
    int triangleWidth = 4;
    int triangleX = centerX - triangleWidth / 2;
    int triangleY = centerY - triangleHeight / 2;
    
    // Draw a simple right-pointing triangle
    for (int y = 0; y < triangleHeight; y++) {
        // Calculate how many pixels to draw for this row
        int pixelsToDraw = (y < triangleHeight / 2) ? y + 1 : triangleHeight - y;
        
        // Draw pixels from left edge
        for (int x = 0; x < pixelsToDraw; x++) {
            offscreen_->SetPixel(triangleX + x, triangleY + y, whiteR, whiteG, whiteB);
        }
    }
}

void YoutubeDisplay::drawSubscriberText(int centerX, int startY) {
    if (!fontsLoaded_) return;
    
    // Apply brightness scaling to text color
    int textR = scaleBrightness(Config::Colors::TEXT_R);
    int textG = scaleBrightness(Config::Colors::TEXT_G);
    int textB = scaleBrightness(Config::Colors::TEXT_B);
    rgb_matrix::Color textColor(textR, textG, textB);
    
    // Draw "subscribers" text
    std::string text = "subscribers";
    int textWidth = text.length() * mediumFont_.CharacterWidth('A');
    int textX = centerX - textWidth / 2;
    
    rgb_matrix::DrawText(offscreen_, mediumFont_, textX, startY + mediumFont_.height(), textColor, text.c_str());
}

void YoutubeDisplay::drawSubscriberCount(int subscriberCount, int centerX, int startY) {
    if (!fontsLoaded_) return;
    
    // Apply brightness scaling to text color
    int textR = scaleBrightness(Config::Colors::TEXT_R);
    int textG = scaleBrightness(Config::Colors::TEXT_G);
    int textB = scaleBrightness(Config::Colors::TEXT_B);
    rgb_matrix::Color textColor(textR, textG, textB);
    
    // Format number with commas
    std::string formattedNumber = formatNumber(subscriberCount);
    
    // Draw the number
    int textWidth = formattedNumber.length() * largeFont_.CharacterWidth('0');
    int textX = centerX - textWidth / 2;
    
    rgb_matrix::DrawText(offscreen_, largeFont_, textX, startY + largeFont_.height(), textColor, formattedNumber.c_str());
}

void YoutubeDisplay::setBrightness(int brightnessLevel) {
    if (brightnessLevel >= Config::MIN_BRIGHTNESS && brightnessLevel <= Config::MAX_BRIGHTNESS) {
        brightnessLevel_ = brightnessLevel;
        brightnessScale_ = (brightnessLevel * 255) / Config::MAX_BRIGHTNESS;
    }
}

int YoutubeDisplay::scaleBrightness(int color) const {
    return (color * brightnessScale_) / 255;
}

void YoutubeDisplay::loadFonts() {
    fontsLoaded_ = false;
    
    if (!largeFont_.LoadFont(Config::LARGE_FONT_PATH.c_str())) {
        std::cerr << "Large font not found: " << Config::LARGE_FONT_PATH << std::endl;
        return;
    }
    
    if (!mediumFont_.LoadFont(Config::MEDIUM_FONT_PATH.c_str())) {
        std::cerr << "Medium font not found: " << Config::MEDIUM_FONT_PATH << ", using large font" << std::endl;
        mediumFont_ = largeFont_;
    }
    
    fontsLoaded_ = true;
}

std::string YoutubeDisplay::formatNumber(int number) const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << number;
    std::string numStr = ss.str();
    
    // Add commas for thousands
    std::string result;
    int count = 0;
    for (int i = numStr.length() - 1; i >= 0; i--) {
        if (count > 0 && count % 3 == 0) {
            result = "," + result;
        }
        result = numStr[i] + result;
        count++;
    }
    
    return result;
}
