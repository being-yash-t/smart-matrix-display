#include "youtube_app.h"
#include "led-matrix.h"
#include <iostream>
#include <signal.h>

using namespace rgb_matrix;

YoutubeApp::YoutubeApp(RGBMatrix* matrix, int brightnessLevel) 
    : matrix_(matrix), display_(nullptr), 
      isRunning_(false), currentSubscriberCount_(1105), brightnessLevel_(brightnessLevel) {
}

YoutubeApp::~YoutubeApp() {
    cleanup();
}

bool YoutubeApp::initialize() {
    // Create components
    display_ = new YoutubeDisplay(matrix_, brightnessLevel_);
    
    isRunning_ = true;
    printStartupInfo();
    
    return true;
}

void YoutubeApp::update() {
    if (!isRunning_) {
        return;
    }
    
    // Update display
    display_->update(currentSubscriberCount_);
}

void YoutubeApp::updateValue(int newValue) {
    if (newValue >= 0 && newValue <= 999999999) {
        currentSubscriberCount_ = newValue;
    }
}

void YoutubeApp::cleanup() {
    if (display_) {
        delete display_;
        display_ = nullptr;
    }
    
    isRunning_ = false;
}

void YoutubeApp::setBrightness(int brightnessLevel) {
    if (brightnessLevel >= 1 && brightnessLevel <= 10) {
        brightnessLevel_ = brightnessLevel;
        if (display_) {
            display_->setBrightness(brightnessLevel);
        }
    }
}

void YoutubeApp::printStartupInfo() {
    std::cout << "\033[1;36m📺 YouTube Counter Application\033[0m" << std::endl;
    std::cout << "\033[0;33m💡 Brightness:\033[0m " << brightnessLevel_ << "/10 (" << (brightnessLevel_ * 10) << "%)" << std::endl;
    std::cout << "\033[0;32m📊 Enter subscriber count and press Enter:\033[0m" << std::endl;
    std::cout << "\033[0;31m⚠️  Type 'back' to return to main menu\033[0m" << std::endl;
    std::cout << std::endl;
}

void YoutubeApp::setupMatrixOptions(RGBMatrix::Options& options, RuntimeOptions& runtimeOpt) {
    // This method is not used in this app since matrix is passed from main
}
