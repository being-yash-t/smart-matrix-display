#include "led-matrix.h"
#include "graphics.h"
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cstring>
#include <sys/select.h>

using namespace rgb_matrix;

// --- Draw 4-pixel border ---
void drawBorder(Canvas *canvas, int r, int g, int b, int thickness=4) {
    int rows = canvas->height();
    int cols = canvas->width();

    for (int t=0; t<thickness; t++) {
        for (int x=t; x<cols-t; x++) {
            canvas->SetPixel(x, t, r,g,b);               // Top
            canvas->SetPixel(x, rows-1-t, r,g,b);       // Bottom
        }
        for (int y=t; y<rows-t; y++) {
            canvas->SetPixel(t, y, r,g,b);              // Left
            canvas->SetPixel(cols-1-t, y, r,g,b);       // Right
        }
    }
}

// --- Draw numeric DB centered vertically, side by side ---
void drawDbText(Canvas *canvas, int dbValue, int componentStartY) {
    int cols = canvas->width();

    // Load fonts
    rgb_matrix::Font largeFont, smallFont;
    if (!largeFont.LoadFont("../fonts/7x13.bdf")) {
        std::cerr << "Large font not found, skipping text\n";
        return;
    }
    if (!smallFont.LoadFont("../fonts/5x7.bdf")) {
        std::cerr << "Small font not found, using large font\n";
        smallFont = largeFont;
    }

    rgb_matrix::Color white(255,255,255);
    
    // Position text to align with progress bar (left-aligned with progress bar start)
    int border = 4;
    int padding = 6;
    int textX = border + padding; // Same as progress bar start position
    
    // Draw large dB number
    char dbBuf[8];
    snprintf(dbBuf, sizeof(dbBuf), "%d", dbValue);
    rgb_matrix::DrawText(canvas, largeFont, textX, componentStartY + largeFont.height(), white, dbBuf);
    
    // Draw small "dB" unit right after the number
    int unitX = textX + largeFont.CharacterWidth('0') * strlen(dbBuf) + 2; // 2px spacing
    rgb_matrix::DrawText(canvas, smallFont, unitX, componentStartY + largeFont.height(), white, "dB");
}

// --- Draw horizontal VU meter below text with segmented colors ---
void drawVuMeter(Canvas *canvas, int dbValue, int componentStartY) {
    int cols = canvas->width();
    int border = 4;
    int padding = 4;

    int meterHeight = 2;
    int textSpacing = 4; // 4px spacing below text (increased from 2px)

    // Position VU meter 4px below the text (within the component)
    int startY = componentStartY + 15 + textSpacing; // below text with 4px spacing
    
    int meterWidth = cols - 2*border - 2*padding;
    int startX = border + padding;
    
    // Calculate segment positions (assuming 120dB max)
    int greenEnd = (80 * meterWidth) / 120;      // Green up to 80dB
    int yellowEnd = (95 * meterWidth) / 120;     // Yellow from 80-95dB
    int totalFill = (dbValue * meterWidth) / 120; // Total fill based on current dB

    // Draw green segment (0-80dB)
    if(totalFill > 0) {
        int greenFill = (totalFill > greenEnd) ? greenEnd : totalFill;
        for(int x = startX; x < startX + greenFill; x++) {
            for(int h = 0; h < meterHeight; h++) {
                canvas->SetPixel(x, startY + h, 0, 255, 0); // Green
            }
        }
    }

    // Draw yellow segment (80-95dB)
    if(totalFill > greenEnd) {
        int yellowFill = (totalFill > yellowEnd) ? yellowEnd : totalFill;
        for(int x = startX + greenEnd; x < startX + yellowFill; x++) {
            for(int h = 0; h < meterHeight; h++) {
                canvas->SetPixel(x, startY + h, 255, 255, 0); // Yellow
            }
        }
    }

    // Draw red segment (95dB+)
    if(totalFill > yellowEnd) {
        for(int x = startX + yellowEnd; x < startX + totalFill; x++) {
            for(int h = 0; h < meterHeight; h++) {
                canvas->SetPixel(x, startY + h, 255, 0, 0); // Red
            }
        }
    }
}

// --- Read DB value from stdin ---
int getDbInput() {
    int db=-1;
    std::string line;
    std::getline(std::cin, line);
    try { db = std::stoi(line); }
    catch(...) { db=-1; }
    return db;
}

int main(int argc, char **argv) {
    RGBMatrix::Options matrix_options;
    matrix_options.hardware_mapping = "regular";
    matrix_options.rows = 48;
    matrix_options.cols = 96;
    matrix_options.pwm_bits = 11;
    matrix_options.pwm_lsb_nanoseconds = 130;
    matrix_options.brightness = 100;
    matrix_options.disable_hardware_pulsing = true;

    RuntimeOptions runtime_opt;
    runtime_opt.gpio_slowdown = 4;

    RGBMatrix *matrix = CreateMatrixFromOptions(matrix_options, runtime_opt);
    if(!matrix) { std::cerr << "Could not init matrix\n"; return 1; }

    // Create off-screen canvas for double buffering (no flicker)
    FrameCanvas *offscreen = matrix->CreateFrameCanvas();

    std::cout << "Enter dB value (0-120) and press Enter:\n";

    int dbValue = 60; // initial
    int lastDbValue = -1; // track changes
    static bool blinkState = true;
    static bool lastBlinkState = false;

    while(true) {
        // Read new value if entered (non-blocking)
        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        
        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
            int val = getDbInput();
            if(val>=0 && val<=120) dbValue=val;
        }

        // --- Calculate component center (text + progress bar as one unit) ---
        int rows = offscreen->height();
        int componentHeight = 15 + 4 + 2; // text height + spacing (4px) + progress bar height
        int componentStartY = (rows - componentHeight) / 2;

        // --- Determine border colour and blink behavior ---
        int br=200, bg=200, bb=200; // light grey default (below 80)
        useconds_t blinkDuration = 200000;
        bool shouldBlink = false;

        if(dbValue >= 80 && dbValue < 90) {
            // Yellow border, slow fade/blink
            br=255; bg=255; bb=0;
            blinkDuration = 500000; // slow fade
            shouldBlink = true;
        } else if(dbValue >= 90 && dbValue < 95) {
            // Orange border, flashing
            br=255; bg=165; bb=0;
            blinkDuration = 250000; // medium flash
            shouldBlink = true;
        } else if(dbValue >= 95) {
            // Red border, fast flashing
            br=255; bg=0; bb=0;
            blinkDuration = 100000; // fast flash
            shouldBlink = true;
        }

        // Only clear and redraw if dB value changed
        if(dbValue != lastDbValue) {
            offscreen->Clear();
            drawDbText(offscreen, dbValue, componentStartY);
            drawVuMeter(offscreen, dbValue, componentStartY);
            lastDbValue = dbValue;
        }

        // Handle border blinking - only update border if needed
        bool borderShouldBeVisible = !shouldBlink || blinkState;
        bool borderWasVisible = !shouldBlink || lastBlinkState;
        
        if(borderShouldBeVisible != borderWasVisible || dbValue != lastDbValue) {
            if(borderShouldBeVisible) {
                drawBorder(offscreen, br, bg, bb, 4);
            } else {
                // Clear border area by redrawing text and progress bar
                drawDbText(offscreen, dbValue, componentStartY);
                drawVuMeter(offscreen, dbValue, componentStartY);
            }
        }

        // Swap the offscreen canvas with the visible one (double buffering)
        offscreen = matrix->SwapOnVSync(offscreen);

        // Update states
        lastBlinkState = blinkState;
        blinkState = !blinkState;
        usleep(blinkDuration);
    }

    matrix->Clear();
    delete matrix;
    return 0;
}