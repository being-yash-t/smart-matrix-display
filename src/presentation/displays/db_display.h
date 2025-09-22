#ifndef DB_DISPLAY_H
#define DB_DISPLAY_H

#include "led-matrix.h"
#include "graphics.h"
#include "infrastructure/config/config.h"
#include "shared/utils/color_utils.h"
#include "infrastructure/display/border_renderer.h"
#include <string>

using namespace rgb_matrix;

class DbDisplay {
public:
    DbDisplay(RGBMatrix* matrix, int brightnessLevel = Config::DEFAULT_BRIGHTNESS);
    ~DbDisplay();
    
    // Main display update method
    void update(int dbValue, bool blinkState);
    
    // Utility methods
    void setBrightness(int brightnessLevel);
    
    // Border control
    void enableBorder(bool enable = true);
    void disableBorder();
    bool isBorderEnabled() const;
    
private:
    // Drawing methods
    void clearAndRedraw(int dbValue, int componentStartY);
    void drawText(int dbValue, int componentStartY);
    void drawProgressBar(int dbValue, int componentStartY);
    void drawBarSegment(int startX, int startY, int width, int r, int g, int b);
    
    // Helper methods
    int getComponentStartY() const;
    int scaleBrightness(int color) const;
    void loadFonts();
    
    // Member variables
    RGBMatrix* matrix_;
    FrameCanvas* offscreen_;
    int brightnessLevel_;
    int brightnessScale_;
    bool fontsLoaded_;
    bool borderEnabled_;
    
    // Components
    BorderRenderer borderRenderer_;
    
    // Fonts (cached for performance)
    rgb_matrix::Font largeFont_;
    rgb_matrix::Font smallFont_;
};

#endif // DB_DISPLAY_H
