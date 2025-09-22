#ifndef BORDER_RENDERER_H
#define BORDER_RENDERER_H

#include "led-matrix.h"
#include "graphics.h"
#include "../core/config.h"

using namespace rgb_matrix;

class BorderRenderer {
public:
    BorderRenderer();
    ~BorderRenderer();
    
    // Main border drawing method
    void drawBorder(Canvas* canvas, int dbValue, bool shouldShow);
    
    // Border state methods
    bool shouldBlink(int dbValue) const;
    int getBlinkDuration(int dbValue) const;
    
    // Border configuration
    void setThickness(int thickness);
    int getThickness() const;
    
private:
    // Helper methods
    void getBorderColor(int dbValue, int& r, int& g, int& b) const;
    void blendColors(int r1, int g1, int b1, int r2, int g2, int b2, 
                     float ratio, int& outR, int& outG, int& outB) const;
    
    // Member variables
    int borderThickness_;
};

#endif // BORDER_RENDERER_H
