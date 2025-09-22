#include "border_renderer.h"
#include <iostream>

BorderRenderer::BorderRenderer() : borderThickness_(Config::BORDER_THICKNESS) {
}

BorderRenderer::~BorderRenderer() {
}

void BorderRenderer::drawBorder(Canvas* canvas, const ColorUtils::Color& color, bool shouldShow) {
    if (!shouldShow) return;
    
    // Border colors are already inherently dimmer, no additional brightness scaling
    
    int rows = canvas->height();
    int cols = canvas->width();
    
    for (int t = 0; t < borderThickness_; t++) {
        // Top and bottom borders
        for (int x = t; x < cols - t; x++) {
            canvas->SetPixel(x, t, color.r, color.g, color.b);               // Top
            canvas->SetPixel(x, rows - 1 - t, color.r, color.g, color.b);    // Bottom
        }
        // Left and right borders
        for (int y = t; y < rows - t; y++) {
            canvas->SetPixel(t, y, color.r, color.g, color.b);               // Left
            canvas->SetPixel(cols - 1 - t, y, color.r, color.g, color.b);    // Right
        }
    }
}


void BorderRenderer::setThickness(int thickness) {
    if (thickness > 0) {
        borderThickness_ = thickness;
    }
}

int BorderRenderer::getThickness() const {
    return borderThickness_;
}


