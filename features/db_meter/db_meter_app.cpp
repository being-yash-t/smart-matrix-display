#include "db_meter_app.h"
#include <iostream>
#include <signal.h>

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
    interrupt_received = true;
}

DbMeterApp::DbMeterApp(int brightnessLevel) 
    : matrix_(nullptr), display_(nullptr), inputHandler_(nullptr), 
      blinkManager_(nullptr), currentDbValue_(Config::DEFAULT_DB_VALUE), 
      brightnessLevel_(brightnessLevel), isRunning_(false) {
}

DbMeterApp::~DbMeterApp() {
    cleanup();
}

bool DbMeterApp::initialize() {
    // Set up matrix options
    RGBMatrix::Options matrix_options;
    RuntimeOptions runtime_opt;
    setupMatrixOptions(matrix_options, runtime_opt);
    
    // Create matrix
    matrix_ = CreateMatrixFromOptions(matrix_options, runtime_opt);
    if (!matrix_) {
        std::cerr << "\033[0;31m❌ Could not initialize matrix\033[0m" << std::endl;
        return false;
    }
    
    // Create components
    display_ = new DbDisplay(matrix_, brightnessLevel_);
    inputHandler_ = new InputHandler();
    blinkManager_ = new BlinkManager();
    
    // Set up interrupt handlers
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);
    
    isRunning_ = true;
    printStartupInfo();
    
    return true;
}

void DbMeterApp::run() {
    if (!isRunning_) {
        std::cerr << "\033[0;31m❌ Application not initialized\033[0m" << std::endl;
        return;
    }
    
    while (!interrupt_received && isRunning_) {
        // Check for new input
        if (inputHandler_->hasInput()) {
            int newValue = inputHandler_->readDbValue();
            if (inputHandler_->isValidDbValue(newValue)) {
                currentDbValue_ = newValue;
            }
        }
        
        // Update blink state first
        bool currentBlinkState = blinkManager_->updateBlinkState();
        
        // Update display with current blink state
        display_->update(currentDbValue_, currentBlinkState);
        
        // Sleep based on current dB level
        int sleepDuration = display_->getBlinkDuration(currentDbValue_);
        usleep(sleepDuration);
    }
    
    std::cout << "\n\033[0;31m⚠️  Received CTRL-C. Exiting.\033[0m" << std::endl;
}

void DbMeterApp::cleanup() {
    if (display_) {
        delete display_;
        display_ = nullptr;
    }
    
    if (inputHandler_) {
        delete inputHandler_;
        inputHandler_ = nullptr;
    }
    
    if (blinkManager_) {
        delete blinkManager_;
        blinkManager_ = nullptr;
    }
    
    if (matrix_) {
        matrix_->Clear();
        delete matrix_;
        matrix_ = nullptr;
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

void DbMeterApp::setupMatrixOptions(RGBMatrix::Options& options, RuntimeOptions& runtimeOpt) {
    options.hardware_mapping = "regular";
    options.rows = 48;
    options.cols = 96;
    options.pwm_bits = 11;
    options.pwm_lsb_nanoseconds = 130;
    options.brightness = 100;
    options.disable_hardware_pulsing = true;
    
    runtimeOpt.gpio_slowdown = 4;
}

void DbMeterApp::printStartupInfo() {
    std::cout << "\033[1;36m🎵 dB Meter Application - Audio Level Monitor\033[0m" << std::endl;
    std::cout << "\033[0;33m💡 Brightness:\033[0m " << brightnessLevel_ << "/10 (" << (brightnessLevel_ * 10) << "%)" << std::endl;
    std::cout << "\033[0;32m📊 Enter dB values (0-120) and press Enter:\033[0m" << std::endl;
    std::cout << "\033[0;31m⚠️  Press <CTRL-C> to exit\033[0m" << std::endl;
    std::cout << std::endl;
}
