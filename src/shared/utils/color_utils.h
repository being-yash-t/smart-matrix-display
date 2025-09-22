#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

class ColorUtils {
public:
    // Generic color blending function
    static void blendColors(int r1, int g1, int b1, int r2, int g2, int b2, 
                           float ratio, int& outR, int& outG, int& outB);
    
    // Color validation
    static void clampColor(int& r, int& g, int& b);
    
    // Color structure for easy passing
    struct Color {
        int r, g, b;
        Color(int red = 0, int green = 0, int blue = 0) : r(red), g(green), b(blue) {}
    };
};

#endif // COLOR_UTILS_H
