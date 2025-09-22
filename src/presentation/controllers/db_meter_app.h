#ifndef DB_METER_APP_H
#define DB_METER_APP_H

#include "presentation/displays/db_display.h"
#include "infrastructure/input/input_handler.h"
#include "shared/utils/blink_manager.h"
#include "infrastructure/config/config.h"
#include "led-matrix.h"
#include <unistd.h>

class DbMeterApp {
public:
    DbMeterApp(RGBMatrix* matrix, int brightnessLevel = Config::DEFAULT_BRIGHTNESS);
    ~DbMeterApp();
    
    // Initialize the application
    bool initialize();
    
    // Update methods (called by main app)
    void update();
    void updateValue(int newValue);
    
    // Cleanup resources
    void cleanup();
    
    // Configuration
    void setBrightness(int brightnessLevel);
    
private:
    RGBMatrix* matrix_;
    DbDisplay* display_;
    BlinkManager* blinkManager_;
    
    int currentDbValue_;
    int brightnessLevel_;
    bool isRunning_;
    
    // Matrix configuration
    void printStartupInfo();
};

#endif // DB_METER_APP_H
