#include "youtube_app.h"
#include "led-matrix.h"
#include <iostream>
#include <signal.h>

using namespace rgb_matrix;

YoutubeApp::YoutubeApp(RGBMatrix* matrix, int brightnessLevel) 
    : matrix_(matrix), display_(nullptr), 
      isRunning_(false), currentSubscriberCount_(1501), currentPlays_(2000), currentLikes_(300),
      brightnessLevel_(brightnessLevel), rotatingText_(nullptr) {
}

YoutubeApp::~YoutubeApp() {
    cleanup();
}

bool YoutubeApp::initialize() {
    // Create components
    display_ = new YoutubeDisplay(matrix_, brightnessLevel_);
    rotatingText_ = new RotatingText();
    
    // Setup rotating text with different metrics
    rotatingText_->addText(std::to_string(currentSubscriberCount_) + " Subs");
    rotatingText_->addText(std::to_string(currentPlays_) + " Plays");
    rotatingText_->addText(std::to_string(currentLikes_) + " Likes");
    rotatingText_->setRotationInterval(3000); // 3 seconds per text
    rotatingText_->start();
    
    isRunning_ = true;
    printStartupInfo();
    
    return true;
}

void YoutubeApp::update() {
    if (!isRunning_) {
        return;
    }
    
    // Update rotating text
    rotatingText_->update();
    
    // Update display with current rotating text
    display_->update(rotatingText_->getCurrentText());
}

void YoutubeApp::updateValue(int newValue) {
    if (newValue >= 0 && newValue <= 999999999) {
        currentSubscriberCount_ = newValue;
        
        // Update rotating text with new subscriber count
        if (rotatingText_) {
            rotatingText_->stop();
            rotatingText_->addText(std::to_string(currentSubscriberCount_) + " Subs");
            rotatingText_->addText(std::to_string(currentPlays_) + " Plays");
            rotatingText_->addText(std::to_string(currentLikes_) + " Likes");
            rotatingText_->start();
        }
    }
}

void YoutubeApp::cleanup() {
    if (display_) {
        delete display_;
        display_ = nullptr;
    }
    
    if (rotatingText_) {
        delete rotatingText_;
        rotatingText_ = nullptr;
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
