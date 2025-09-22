#include "db_meter_app.h"
#include "db_color_calculator.h"
#include <iostream>
#include <signal.h>

// Interrupt handling is managed by the main app

DbMeterApp::DbMeterApp(RGBMatrix* matrix, int brightnessLevel) 
    : matrix_(matrix), display_(nullptr), 
      blinkManager_(nullptr), currentDbValue_(Config::DEFAULT_DB_VALUE), 
      brightnessLevel_(brightnessLevel), isRunning_(false) {
}

DbMeterApp::~DbMeterApp() {
    cleanup();
}

bool DbMeterApp::initialize() {
    if (!matrix_) {
        std::cerr << "\033[0;31m❌ Matrix not provided\033[0m" << std::endl;
        return false;
    }
    
    // Create components
    display_ = new DbDisplay(matrix_, brightnessLevel_);
    blinkManager_ = new BlinkManager();
    
    // Interrupt handlers are managed by the main app
    
    isRunning_ = true;
    printStartupInfo();
    
    return true;
}

void DbMeterApp::update() {
    if (!isRunning_) {
        return;
    }
    
    // Get blink duration based on dB level
    int blinkDuration = DbColorCalculator::getBlinkDuration(currentDbValue_);
    
    // Update blink state with duration
    bool currentBlinkState = blinkManager_->updateBlinkState(blinkDuration);
    
    // Update display with current blink state
    display_->update(currentDbValue_, currentBlinkState);
}

void DbMeterApp::updateValue(int newValue) {
    if (newValue >= 0 && newValue <= 120) {
        currentDbValue_ = newValue;
    }
}

void DbMeterApp::cleanup() {
    if (display_) {
        delete display_;
        display_ = nullptr;
    }
    
    
    if (blinkManager_) {
        delete blinkManager_;
        blinkManager_ = nullptr;
    }
    
    if (matrix_) {
        matrix_->Clear();
        // Don't delete matrix_ - it's managed by the main app
    }
    
    isRunning_ = false;
}

void DbMeterApp::setBrightness(int brightnessLevel) {
    if (brightnessLevel >= Config::MIN_BRIGHTNESS && brightnessLevel <= Config::MAX_BRIGHTNESS) {
        brightnessLevel_ = brightnessLevel;
        if (display_) {
            display_->setBrightness(brightnessLevel);
        }
    }
}


void DbMeterApp::printStartupInfo() {
    std::cout << "\033[1;36m🎵 dB Meter Application - Audio Level Monitor\033[0m" << std::endl;
    std::cout << "\033[0;33m💡 Brightness:\033[0m " << brightnessLevel_ << "/10 (" << (brightnessLevel_ * 10) << "%)" << std::endl;
    std::cout << "\033[0;32m📊 Enter dB values (0-120) and press Enter:\033[0m" << std::endl;
    std::cout << "\033[0;31m⚠️  Type 'back' to return to main menu\033[0m" << std::endl;
    std::cout << std::endl;
}
