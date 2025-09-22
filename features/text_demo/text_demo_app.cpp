#include "text_demo_app.h"
#include "led-matrix.h"
#include <iostream>
#include <signal.h>

using namespace rgb_matrix;

// Global flag for interrupt handling
volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
    interrupt_received = true;
}

TextDemoApp::TextDemoApp(int brightnessLevel) 
    : matrix_(nullptr), display_(nullptr), inputHandler_(nullptr), 
      isRunning_(false), currentText_("Hello World!"), brightnessLevel_(brightnessLevel) {
}

TextDemoApp::~TextDemoApp() {
    cleanup();
}

bool TextDemoApp::initialize() {
    // Set up interrupt handler
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);
    
    // Create matrix options
    RGBMatrix::Options matrix_options;
    RuntimeOptions runtime_opt;
    
    // Configure matrix
    matrix_options.rows = 32;
    matrix_options.cols = 64;
    matrix_options.chain_length = 1;
    matrix_options.parallel = 1;
    matrix_options.hardware_mapping = "regular";
    matrix_options.brightness = brightnessLevel_ * 10; // Convert 1-10 to 10-100
    matrix_options.disable_hardware_pulsing = true;
    
    runtime_opt.gpio_slowdown = 4;
    
    // Create matrix
    matrix_ = CreateMatrixFromOptions(matrix_options, runtime_opt);
    if (!matrix_) {
        std::cerr << "\033[0;31m❌ Could not initialize matrix\033[0m" << std::endl;
        return false;
    }
    
    // Create components
    display_ = new TextDisplay(matrix_, brightnessLevel_);
    inputHandler_ = new InputHandler();
    
    isRunning_ = true;
    printStartupInfo();
    return true;
}

void TextDemoApp::run() {
    if (!isRunning_) {
        std::cerr << "\033[0;31m❌ Application not initialized\033[0m" << std::endl;
        return;
    }
    
    while (!interrupt_received && isRunning_) {
        // Check for new input
        std::string input = inputHandler_->getInput();
        if (!input.empty()) {
            currentText_ = input;
        }
        
        // Update display
        display_->update(currentText_);
        
        // Small delay to prevent excessive CPU usage
        usleep(50000); // 50ms
    }
    
    std::cout << "\n\033[0;31m⚠️  Received CTRL-C. Exiting.\033[0m" << std::endl;
}

void TextDemoApp::cleanup() {
    if (display_) {
        delete display_;
        display_ = nullptr;
    }
    
    if (inputHandler_) {
        delete inputHandler_;
        inputHandler_ = nullptr;
    }
    
    if (matrix_) {
        delete matrix_;
        matrix_ = nullptr;
    }
}

void TextDemoApp::printStartupInfo() {
    std::cout << "\033[1;36m📝 Text Demo Application - Simple Text Display\033[0m" << std::endl;
    std::cout << "\033[0;33m💡 Brightness:\033[0m " << brightnessLevel_ << "/10 (" << (brightnessLevel_ * 10) << "%)" << std::endl;
    std::cout << "\033[0;32m📊 Enter text and press Enter to display:\033[0m" << std::endl;
    std::cout << "\033[0;31m⚠️  Press <CTRL-C> to exit\033[0m" << std::endl;
    std::cout << std::endl;
}
