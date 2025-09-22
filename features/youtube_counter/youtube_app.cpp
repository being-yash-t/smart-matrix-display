#include "youtube_app.h"
#include "led-matrix.h"
#include <iostream>
#include <signal.h>
#include <sstream>
#include <iomanip>

using namespace rgb_matrix;

YoutubeApp::YoutubeApp(RGBMatrix* matrix, int brightnessLevel) 
    : matrix_(matrix), display_(nullptr), 
      isRunning_(false), currentSubscriberCount_(0), currentViewCount_(0), currentVideoCount_(0),
      brightnessLevel_(brightnessLevel), youtubeAPI_(new YouTubeAPI()),
      channelId_("@being_jay_thakur"), isLoading_(false), hasError_(false), rotatingText_(nullptr) {
}

YoutubeApp::~YoutubeApp() {
    cleanup();
}

bool YoutubeApp::initialize() {
    // Create components
    display_ = new YoutubeDisplay(matrix_, brightnessLevel_);
    rotatingText_ = new RotatingText();
    
    // Check if API is configured
    if (!youtubeAPI_->isConfigured()) {
        setErrorState("API key not configured");
        std::cerr << "YouTube API not configured. Set YOUTUBE_API_KEY environment variable." << std::endl;
    } else {
        // Start with loading state and automatically fetch data for hardcoded channel
        setLoadingState();
        std::cout << "🔍 Auto-fetching data for @being_jay_thakur..." << std::endl;
        refreshData();
    }
    
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
    // This method is deprecated - use setChannelId and refreshData instead
    std::cout << "Note: updateValue is deprecated. Use setChannelId() and refreshData() instead." << std::endl;
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
    
    if (youtubeAPI_) {
        delete youtubeAPI_;
        youtubeAPI_ = nullptr;
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
    std::cout << "\033[0;32m📊 Channel:\033[0m @being_jay_thakur (auto-loaded)" << std::endl;
    std::cout << "\033[0;34m🔄 Press 'r' to refresh data\033[0m" << std::endl;
    std::cout << "\033[0;31m⚠️  Type 'back' to return to main menu\033[0m" << std::endl;
    std::cout << std::endl;
}

void YoutubeApp::setupMatrixOptions(RGBMatrix::Options& options, RuntimeOptions& runtimeOpt) {
    // This method is not used in this app since matrix is passed from main
}

void YoutubeApp::setChannelId(const std::string& channelId) {
    channelId_ = channelId;
    if (!channelId_.empty()) {
        refreshData();
    }
}

void YoutubeApp::refreshData() {
    if (channelId_.empty()) {
        setErrorState("No channel ID set");
        return;
    }
    
    if (!youtubeAPI_->isConfigured()) {
        setErrorState("API key not configured");
        return;
    }
    
    setLoadingState();
    
    // Make API call - check if it's a username (starts with @) or channel ID
    YouTubeChannelStats stats;
    if (channelId_.empty()) {
        stats.errorMessage = "No channel ID set";
    } else if (channelId_[0] == '@') {
        // It's a username, remove the @ and use getChannelStatsByUsername
        std::string username = channelId_.substr(1); // Remove the @
        std::cout << "🔍 Looking up username: " << username << std::endl;
        stats = youtubeAPI_->getChannelStatsByUsername(username);
    } else {
        // It's a channel ID, use getChannelStats
        std::cout << "🔍 Looking up channel ID: " << channelId_ << std::endl;
        stats = youtubeAPI_->getChannelStats(channelId_);
    }
    
    if (stats.isValid) {
        setDataState(stats);
        std::cout << "✅ Data refreshed successfully!" << std::endl;
    } else {
        setErrorState(stats.errorMessage);
        std::cerr << "❌ Error refreshing data: " << stats.errorMessage << std::endl;
    }
}

void YoutubeApp::handleKeyboardInput(char key) {
    switch (key) {
        case 'r':
        case 'R':
            if (!channelId_.empty()) {
                std::cout << "🔄 Refreshing data..." << std::endl;
                refreshData();
            } else {
                std::cout << "⚠️  Please set a channel ID first" << std::endl;
            }
            break;
        default:
            // Handle channel ID input
            if (key >= 32 && key <= 126) { // Printable characters
                if (channelId_.length() < 50) { // Reasonable limit
                    channelId_ += key;
                    std::cout << "Channel ID: " << channelId_ << std::endl;
                }
            } else if (key == '\n' || key == '\r') { // Enter key
                if (!channelId_.empty()) {
                    std::cout << "🔍 Fetching data for channel: " << channelId_ << std::endl;
                    refreshData();
                }
            } else if (key == '\b' || key == 127) { // Backspace
                if (!channelId_.empty()) {
                    channelId_.pop_back();
                    std::cout << "Channel ID: " << channelId_ << std::endl;
                }
            }
            break;
    }
}

void YoutubeApp::updateRotatingText() {
    if (!rotatingText_) return;
    
    rotatingText_->stop();
    rotatingText_->clearTexts();
    
    if (isLoading_) {
        rotatingText_->addText("Loading...");
    } else if (hasError_) {
        rotatingText_->addText("Error!");
        if (!errorMessage_.empty()) {
            rotatingText_->addText(errorMessage_);
        }
    } else {
        // Add formatted data
        rotatingText_->addText(formatNumber(currentSubscriberCount_) + " Subs");
        rotatingText_->addText(formatNumber(currentViewCount_) + " Views");
        rotatingText_->addText(formatNumber(currentVideoCount_) + " Videos");
    }
    
    rotatingText_->start();
}

void YoutubeApp::setLoadingState() {
    isLoading_ = true;
    hasError_ = false;
    errorMessage_.clear();
    updateRotatingText();
}

void YoutubeApp::setErrorState(const std::string& error) {
    isLoading_ = false;
    hasError_ = true;
    errorMessage_ = error;
    updateRotatingText();
}

void YoutubeApp::setDataState(const YouTubeChannelStats& stats) {
    isLoading_ = false;
    hasError_ = false;
    errorMessage_.clear();
    
    currentSubscriberCount_ = stats.subscriberCount;
    currentViewCount_ = stats.viewCount;
    currentVideoCount_ = stats.videoCount;
    
    updateRotatingText();
}

std::string YoutubeApp::formatNumber(long number) const {
    if (number >= 1000000000) {
        return std::to_string(number / 1000000000) + "B";
    } else if (number >= 1000000) {
        return std::to_string(number / 1000000) + "M";
    } else if (number >= 1000) {
        return std::to_string(number / 1000) + "K";
    } else {
        return std::to_string(number);
    }
}
