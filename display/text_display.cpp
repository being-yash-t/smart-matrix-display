#include "text_display.h"
#include <iostream>
#include <cstring>

TextDisplay::TextDisplay(RGBMatrix* matrix, int brightnessLevel) 
    : matrix_(matrix), brightnessLevel_(brightnessLevel), fontLoaded_(false),
      textR_(Config::Colors::TEXT_R), textG_(Config::Colors::TEXT_G), textB_(Config::Colors::TEXT_B) {
    offscreen_ = matrix_->CreateFrameCanvas();
    brightnessScale_ = (brightnessLevel * 255) / Config::MAX_BRIGHTNESS;
    loadFonts();
}

TextDisplay::~TextDisplay() {
    // FrameCanvas is managed by the matrix, no need to delete
}

void TextDisplay::update(const std::string& text) {
    // Clear and redraw everything
    clearAndRedraw(text);
    
    // Swap the offscreen canvas with the visible one (double buffering)
    offscreen_ = matrix_->SwapOnVSync(offscreen_);
}

void TextDisplay::clearAndRedraw(const std::string& text) {
    offscreen_->Clear();
    drawText(text);
}

void TextDisplay::drawText(const std::string& text) {
    if (!fontLoaded_ || text.empty()) return;
    
    // Apply brightness scaling to text colors
    int r = scaleBrightness(textR_);
    int g = scaleBrightness(textG_);
    int b = scaleBrightness(textB_);
    rgb_matrix::Color textColor(r, g, b);
    
    // Calculate text position (centered)
    int textWidth = text.length() * font_.CharacterWidth('A');
    int textHeight = font_.height();
    int startX = (offscreen_->width() - textWidth) / 2;
    int startY = getTextStartY();
    
    // Draw the text
    rgb_matrix::DrawText(offscreen_, font_, startX, startY, textColor, text.c_str());
}

int TextDisplay::getTextStartY() const {
    int rows = offscreen_->height();
    int textHeight = fontLoaded_ ? font_.height() : 10;
    return (rows + textHeight) / 2; // Vertically centered
}

void TextDisplay::setBrightness(int brightnessLevel) {
    if (brightnessLevel >= Config::MIN_BRIGHTNESS && brightnessLevel <= Config::MAX_BRIGHTNESS) {
        brightnessLevel_ = brightnessLevel;
        brightnessScale_ = (brightnessLevel * 255) / Config::MAX_BRIGHTNESS;
    }
}

void TextDisplay::setTextColor(int r, int g, int b) {
    textR_ = r;
    textG_ = g;
    textB_ = b;
}

int TextDisplay::scaleBrightness(int color) const {
    return (color * brightnessScale_) / 255;
}

void TextDisplay::loadFonts() {
    fontLoaded_ = false;
    
    if (!font_.LoadFont(Config::LARGE_FONT_PATH.c_str())) {
        std::cerr << "Font not found: " << Config::LARGE_FONT_PATH << std::endl;
        return;
    }
    
    fontLoaded_ = true;
}
