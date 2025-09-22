#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "led-matrix.h"
#include "graphics.h"
#include "infrastructure/config/config.h"
#include <string>

using namespace rgb_matrix;

class TextDisplay {
public:
    TextDisplay(RGBMatrix* matrix, int brightnessLevel = Config::DEFAULT_BRIGHTNESS);
    ~TextDisplay();
    
    // Main display update method
    void update(const std::string& text);
    
    // Utility methods
    void setBrightness(int brightnessLevel);
    void setTextColor(int r, int g, int b);
    
private:
    // Drawing methods
    void clearAndRedraw(const std::string& text);
    void drawText(const std::string& text);
    
    // Helper methods
    int getTextStartY() const;
    int scaleBrightness(int color) const;
    void loadFonts();
    
    // Member variables
    RGBMatrix* matrix_;
    FrameCanvas* offscreen_;
    int brightnessLevel_;
    int brightnessScale_;
    
    // Text color
    int textR_, textG_, textB_;
    
    // Fonts (cached for performance)
    rgb_matrix::Font font_;
    bool fontLoaded_;
};

#endif // TEXT_DISPLAY_H
