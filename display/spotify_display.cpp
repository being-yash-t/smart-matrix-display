#include "spotify_display.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

SpotifyDisplay::SpotifyDisplay(RGBMatrix* matrix, int brightnessLevel) 
    : matrix_(matrix), brightnessLevel_(brightnessLevel), fontsLoaded_(false) {
    offscreen_ = matrix_->CreateFrameCanvas();
    brightnessScale_ = (brightnessLevel * 255) / Config::MAX_BRIGHTNESS;
    loadFonts();
}

SpotifyDisplay::~SpotifyDisplay() {
    // FrameCanvas is managed by the matrix, no need to delete
}

void SpotifyDisplay::update(const std::string& text) {
    // Clear and redraw everything
    clearAndRedraw(text);
    
    // Swap the offscreen canvas with the visible one (double buffering)
    offscreen_ = matrix_->SwapOnVSync(offscreen_);
}

void SpotifyDisplay::clearAndRedraw(const std::string& text) {
    offscreen_->Clear();
    
    int centerX = offscreen_->width() / 2;
    int screenHeight = offscreen_->height();
    
    // Calculate total height of the unit (icon + gap + text)
    int iconHeight = 13; // Spotify icon height
    int gap = 2; // Gap between icon and text
    int textHeight = mediumFont_.height(); // Text height
    int totalUnitHeight = iconHeight + gap + textHeight;
    
    // Center the entire unit vertically
    int unitStartY = (screenHeight - totalUnitHeight) / 2;
    
    // Draw Spotify logo
    int iconWidth = 18; // Spotify icon width
    int iconStartX = centerX - iconWidth / 2;
    int iconY = unitStartY;
    drawSpotifyLogo(iconStartX, iconY);
    
    // Draw text below with gap
    int textY = iconY + iconHeight + gap;
    drawText(text, centerX, textY);
}

void SpotifyDisplay::drawSpotifyLogo(int startX, int startY) {
    // Apply brightness scaling to colors
    int greenR = scaleBrightness(30);   // Spotify green
    int greenG = scaleBrightness(215);
    int greenB = scaleBrightness(96);
    
    int whiteR = scaleBrightness(255); // White
    int whiteG = scaleBrightness(255);
    int whiteB = scaleBrightness(255);
    
    // Draw Spotify logo - simplified version with circles
    int centerX = startX + 9;
    int centerY = startY + 6;
    
    // Draw three overlapping circles to represent Spotify logo
    // Large circle (background)
    for (int y = 0; y < 13; y++) {
        for (int x = 0; x < 18; x++) {
            int dx = x - 9;
            int dy = y - 6;
            int dist = dx * dx + dy * dy;
            if (dist <= 36) { // radius 6
                offscreen_->SetPixel(startX + x, startY + y, greenR, greenG, greenB);
            }
        }
    }
    
    // Medium circle (middle)
    for (int y = 0; y < 13; y++) {
        for (int x = 0; x < 18; x++) {
            int dx = x - 9;
            int dy = y - 6;
            int dist = dx * dx + dy * dy;
            if (dist <= 16 && dist > 9) { // radius 4, inner radius 3
                offscreen_->SetPixel(startX + x, startY + y, whiteR, whiteG, whiteB);
            }
        }
    }
    
    // Small circle (center)
    for (int y = 0; y < 13; y++) {
        for (int x = 0; x < 18; x++) {
            int dx = x - 9;
            int dy = y - 6;
            int dist = dx * dx + dy * dy;
            if (dist <= 4) { // radius 2
                offscreen_->SetPixel(startX + x, startY + y, greenR, greenG, greenB);
            }
        }
    }
}

void SpotifyDisplay::drawText(const std::string& text, int centerX, int startY) {
    if (!fontsLoaded_ || text.empty()) return;
    
    // Apply brightness scaling to text color
    int textR = scaleBrightness(Config::Colors::TEXT_R);
    int textG = scaleBrightness(Config::Colors::TEXT_G);
    int textB = scaleBrightness(Config::Colors::TEXT_B);
    rgb_matrix::Color textColor(textR, textG, textB);
    
    // Calculate text width and position
    int textWidth = text.length() * mediumFont_.CharacterWidth('A');
    int textX = centerX - textWidth / 2;
    
    // Draw the text
    rgb_matrix::DrawText(offscreen_, mediumFont_, textX, startY + mediumFont_.height(), textColor, text.c_str());
}

void SpotifyDisplay::setBrightness(int brightnessLevel) {
    if (brightnessLevel >= Config::MIN_BRIGHTNESS && brightnessLevel <= Config::MAX_BRIGHTNESS) {
        brightnessLevel_ = brightnessLevel;
        brightnessScale_ = (brightnessLevel * 255) / Config::MAX_BRIGHTNESS;
    }
}

int SpotifyDisplay::scaleBrightness(int color) const {
    return (color * brightnessScale_) / 255;
}

void SpotifyDisplay::loadFonts() {
    fontsLoaded_ = false;
    
    if (!mediumFont_.LoadFont(Config::LARGE_FONT_PATH.c_str())) {
        std::cerr << "Large font not found: " << Config::LARGE_FONT_PATH << std::endl;
        return;
    }
    
    if (!mediumFont_.LoadFont(Config::MEDIUM_FONT_PATH.c_str())) {
        std::cerr << "Medium font not found: " << Config::MEDIUM_FONT_PATH << ", using large font" << std::endl;
        mediumFont_ = mediumFont_;
    }
    
    fontsLoaded_ = true;
}

std::string SpotifyDisplay::formatNumber(int number) const {
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
