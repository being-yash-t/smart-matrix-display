#include "db_display.h"
#include <iostream>
#include <cstring>

DbDisplay::DbDisplay(RGBMatrix* matrix) 
    : matrix_(matrix), lastDbValue_(-1), lastBlinkState_(false) {
    offscreen_ = matrix_->CreateFrameCanvas();
}

DbDisplay::~DbDisplay() {
    // FrameCanvas is managed by the matrix, no need to delete
}

void DbDisplay::update(int dbValue, bool blinkState) {
    int componentStartY = getComponentStartY();
    
    // Always clear and redraw everything to avoid buffer issues
    offscreen_->Clear();
    drawText(dbValue, componentStartY);
    drawProgressBar(dbValue, componentStartY);
    
    // Handle border blinking
    bool shouldBlinkValue = shouldBlink(dbValue);
    bool borderShouldBeVisible = !shouldBlinkValue || blinkState;
    
    if(borderShouldBeVisible) {
        int r, g, b;
        getBorderColor(dbValue, r, g, b);
        drawBorder(r, g, b, BORDER_THICKNESS);
    }
    
    // Swap the offscreen canvas with the visible one (double buffering)
    offscreen_ = matrix_->SwapOnVSync(offscreen_);
    
    lastDbValue_ = dbValue;
    lastBlinkState_ = blinkState;
}

void DbDisplay::drawText(int dbValue, int componentStartY) {
    
    // Load fonts
    rgb_matrix::Font largeFont, smallFont;
    if (!largeFont.LoadFont("../../fonts/7x13.bdf")) {
        std::cerr << "Large font not found, skipping text\n";
        return;
    }
    if (!smallFont.LoadFont("../../fonts/5x7.bdf")) {
        std::cerr << "Small font not found, using large font\n";
        smallFont = largeFont;
    }
    
    rgb_matrix::Color white(255, 255, 255);
    
    // Position text to align with progress bar (left-aligned with progress bar start)
    int textX = BORDER_THICKNESS + PADDING; // Same as progress bar start position
    
    // Draw large dB number
    char dbBuf[8];
    snprintf(dbBuf, sizeof(dbBuf), "%d", dbValue);
    rgb_matrix::DrawText(offscreen_, largeFont, textX, componentStartY + largeFont.height(), white, dbBuf);
    
    // Draw small "dB" unit right after the number
    int unitX = textX + largeFont.CharacterWidth('0') * strlen(dbBuf) + 2; // 2px spacing
    rgb_matrix::DrawText(offscreen_, smallFont, unitX, componentStartY + largeFont.height(), white, "dB");
}

void DbDisplay::drawProgressBar(int dbValue, int componentStartY) {
    int cols = offscreen_->width();
    
    int meterHeight = PROGRESS_BAR_HEIGHT;
    int textSpacing = TEXT_SPACING;
    
    // Position VU meter 4px below the text (within the component)
    int startY = componentStartY + 15 + textSpacing; // below text with 4px spacing
    
    int meterWidth = cols - 2 * BORDER_THICKNESS - 2 * PADDING;
    int startX = BORDER_THICKNESS + PADDING;
    
    // Calculate segment positions (assuming 120dB max)
    int greenEnd = (80 * meterWidth) / 120;      // Green up to 80dB
    int yellowEnd = (95 * meterWidth) / 120;     // Yellow from 80-95dB
    int totalFill = (dbValue * meterWidth) / 120; // Total fill based on current dB
    
    // Draw green segment (0-80dB)
    if(totalFill > 0) {
        int greenFill = (totalFill > greenEnd) ? greenEnd : totalFill;
        for(int x = startX; x < startX + greenFill; x++) {
            for(int h = 0; h < meterHeight; h++) {
                offscreen_->SetPixel(x, startY + h, 0, 255, 0); // Green
            }
        }
    }
    
    // Draw yellow segment (80-95dB)
    if(totalFill > greenEnd) {
        int yellowFill = (totalFill > yellowEnd) ? yellowEnd : totalFill;
        for(int x = startX + greenEnd; x < startX + yellowFill; x++) {
            for(int h = 0; h < meterHeight; h++) {
                offscreen_->SetPixel(x, startY + h, 255, 255, 0); // Yellow
            }
        }
    }
    
    // Draw red segment (95dB+)
    if(totalFill > yellowEnd) {
        for(int x = startX + yellowEnd; x < startX + totalFill; x++) {
            for(int h = 0; h < meterHeight; h++) {
                offscreen_->SetPixel(x, startY + h, 255, 0, 0); // Red
            }
        }
    }
}

void DbDisplay::drawBorder(int r, int g, int b, int thickness) {
    int rows = offscreen_->height();
    int cols = offscreen_->width();
    
    for (int t = 0; t < thickness; t++) {
        for (int x = t; x < cols - t; x++) {
            offscreen_->SetPixel(x, t, r, g, b);               // Top
            offscreen_->SetPixel(x, rows - 1 - t, r, g, b);    // Bottom
        }
        for (int y = t; y < rows - t; y++) {
            offscreen_->SetPixel(t, y, r, g, b);               // Left
            offscreen_->SetPixel(cols - 1 - t, y, r, g, b);    // Right
        }
    }
}

int DbDisplay::getComponentStartY() {
    int rows = offscreen_->height();
    return (rows - COMPONENT_HEIGHT) / 2;
}

bool DbDisplay::shouldBlink(int dbValue) {
    return dbValue >= 80;
}

int DbDisplay::getBlinkDuration(int dbValue) {
    if(dbValue >= 80 && dbValue < 90) {
        return 500000; // slow fade
    } else if(dbValue >= 90 && dbValue < 95) {
        return 250000; // medium flash
    } else if(dbValue >= 95) {
        return 100000; // fast flash
    }
    return 200000; // default
}

void DbDisplay::getBorderColor(int dbValue, int& r, int& g, int& b) {
    r = 200; g = 200; b = 200; // light grey default (below 80)
    
    if(dbValue >= 80 && dbValue < 90) {
        r = 255; g = 255; b = 0; // yellow
    } else if(dbValue >= 90 && dbValue < 95) {
        r = 255; g = 165; b = 0; // orange
    } else if(dbValue >= 95) {
        r = 255; g = 0; b = 0; // red
    }
}
