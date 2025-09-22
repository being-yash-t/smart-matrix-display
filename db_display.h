#ifndef DB_DISPLAY_H
#define DB_DISPLAY_H

#include "led-matrix.h"
#include "graphics.h"
#include "config.h"
#include <string>

using namespace rgb_matrix;

class DbDisplay {
public:
    DbDisplay(RGBMatrix* matrix, int brightnessLevel = Config::DEFAULT_BRIGHTNESS);
    ~DbDisplay();
    
    // Main display update method
    void update(int dbValue, bool blinkState);
    
    // Utility methods
    int getBlinkDuration(int dbValue) const;
    void setBrightness(int brightnessLevel);
    
private:
    // Drawing methods
    void clearAndRedraw(int dbValue, int componentStartY);
    void drawText(int dbValue, int componentStartY);
    void drawProgressBar(int dbValue, int componentStartY);
    void drawBorder(int dbValue, bool shouldShow);
    
    // Helper methods
    int getComponentStartY() const;
    bool shouldBlink(int dbValue) const;
    void getBorderColor(int dbValue, int& r, int& g, int& b) const;
    int scaleBrightness(int color) const;
    
    // Member variables
    RGBMatrix* matrix_;
    FrameCanvas* offscreen_;
    int brightnessLevel_;
    int brightnessScale_;
    
    // Fonts (cached for performance)
    rgb_matrix::Font largeFont_;
    rgb_matrix::Font smallFont_;
    bool fontsLoaded_;
};

#endif // DB_DISPLAY_H
