#ifndef DB_METER_APP_H
#define DB_METER_APP_H

#include "db_display.h"
#include "input_handler.h"
#include "blink_manager.h"
#include "config.h"
#include "led-matrix.h"
#include <unistd.h>

class DbMeterApp {
public:
    DbMeterApp(int brightnessLevel = Config::DEFAULT_BRIGHTNESS);
    ~DbMeterApp();
    
    // Initialize the application
    bool initialize();
    
    // Run the main application loop
    void run();
    
    // Cleanup resources
    void cleanup();
    
    // Configuration
    void setBrightness(int brightnessLevel);
    
private:
    RGBMatrix* matrix_;
    DbDisplay* display_;
    InputHandler* inputHandler_;
    BlinkManager* blinkManager_;
    
    int currentDbValue_;
    int brightnessLevel_;
    bool isRunning_;
    
    // Matrix configuration
    void setupMatrixOptions(RGBMatrix::Options& options, RuntimeOptions& runtimeOpt);
    void printStartupInfo();
};

#endif // DB_METER_APP_H
