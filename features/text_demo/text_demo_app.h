#ifndef TEXT_DEMO_APP_H
#define TEXT_DEMO_APP_H

#include "../../display/text_display.h"
#include "../../core/input_handler.h"
#include "../../core/arg_parser.h"
#include <string>

class TextDemoApp {
public:
    TextDemoApp(int brightnessLevel = 5);
    ~TextDemoApp();
    
    // Application lifecycle
    bool initialize();
    void run();
    void cleanup();
    
private:
    // Components
    RGBMatrix* matrix_;
    TextDisplay* display_;
    InputHandler* inputHandler_;
    
    // State
    bool isRunning_;
    std::string currentText_;
    int brightnessLevel_;
    
    // Helper methods
    void printStartupInfo();
};

#endif // TEXT_DEMO_APP_H
