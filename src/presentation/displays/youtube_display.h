#ifndef YOUTUBE_DISPLAY_H
#define YOUTUBE_DISPLAY_H

#include "led-matrix.h"
#include "graphics.h"
#include "infrastructure/config/config.h"
#include <string>

using namespace rgb_matrix;

class YoutubeDisplay {
public:
    YoutubeDisplay(RGBMatrix* matrix, int brightnessLevel = Config::DEFAULT_BRIGHTNESS);
    ~YoutubeDisplay();
    
    // Main display update method
    void update(const std::string& text);
    
    // Utility methods
    void setBrightness(int brightnessLevel);
    
private:
    // Drawing methods
    void clearAndRedraw(const std::string& text);
    void drawPlayButton(int startX, int startY);
    void drawText(const std::string& text, int centerX, int startY);
    
    // Helper methods
    int scaleBrightness(int color) const;
    void loadFonts();
    std::string formatNumber(int number) const;
    
    // Member variables
    RGBMatrix* matrix_;
    FrameCanvas* offscreen_;
    int brightnessLevel_;
    int brightnessScale_;
    
    // Fonts (cached for performance)
    rgb_matrix::Font largeFont_;
    rgb_matrix::Font mediumFont_;
    bool fontsLoaded_;
};

#endif // YOUTUBE_DISPLAY_H
