#ifndef BORDER_RENDERER_H
#define BORDER_RENDERER_H

#include "led-matrix.h"
#include "graphics.h"
#include "infrastructure/config/config.h"
#include "shared/utils/color_utils.h"

using namespace rgb_matrix;

class BorderRenderer {
public:
    BorderRenderer();
    ~BorderRenderer();
    
    // Main border drawing method (generic - accepts pre-calculated color)
    void drawBorder(Canvas* canvas, const ColorUtils::Color& color, bool shouldShow);
    
    // Border configuration
    void setThickness(int thickness);
    int getThickness() const;
    
private:
    // Member variables
    int borderThickness_;
};

#endif // BORDER_RENDERER_H
