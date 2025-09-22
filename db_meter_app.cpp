#include "db_meter_app.h"
#include <iostream>
#include <signal.h>

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
    interrupt_received = true;
}

DbMeterApp::DbMeterApp() 
    : matrix_(nullptr), display_(nullptr), inputHandler_(nullptr), 
      blinkManager_(nullptr), currentDbValue_(DEFAULT_DB_VALUE), isRunning_(false) {
}

DbMeterApp::~DbMeterApp() {
    cleanup();
}

bool DbMeterApp::initialize() {
    // Set up matrix options
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
    
    // Create matrix
    matrix_ = CreateMatrixFromOptions(matrix_options, runtime_opt);
    if (!matrix_) {
        std::cerr << "Could not initialize matrix\n";
        return false;
    }
    
    // Create components
    display_ = new DbDisplay(matrix_);
    inputHandler_ = new InputHandler();
    blinkManager_ = new BlinkManager();
    
    // Set up interrupt handlers
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);
    
    isRunning_ = true;
    std::cout << "Enter dB value (0-120) and press Enter:\n";
    std::cout << "Press <CTRL-C> to exit\n";
    
    return true;
}

void DbMeterApp::run() {
    if (!isRunning_) {
        std::cerr << "Application not initialized\n";
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
    
    std::cout << "\nReceived CTRL-C. Exiting.\n";
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
