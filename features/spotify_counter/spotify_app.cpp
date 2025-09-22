#include "spotify_app.h"
#include "led-matrix.h"
#include <iostream>
#include <signal.h>
#include <sstream>
#include <iomanip>

using namespace rgb_matrix;

SpotifyApp::SpotifyApp(RGBMatrix* matrix, int brightnessLevel) 
    : matrix_(matrix), display_(nullptr), 
      isRunning_(false), artistName_(""), popularity_(0), monthlyListeners_(0), 
      albumCount_(0), trackCount_(0), topTrack_(""), genres_(""),
      brightnessLevel_(brightnessLevel), spotifyAPI_(new SpotifyAPI()),
      artistId_("6m4ysuZf9XxRhqeujYp5ti"), isLoading_(false), hasError_(false), rotatingText_(nullptr) {
}

SpotifyApp::~SpotifyApp() {
    cleanup();
}

bool SpotifyApp::initialize() {
    // Create components
    display_ = new SpotifyDisplay(matrix_, brightnessLevel_);
    rotatingText_ = new RotatingText();
    
    // Check if API is configured
    if (!spotifyAPI_->isConfigured()) {
        setErrorState("API credentials not configured");
        std::cerr << "Spotify API not configured. Set SPOTIFY_CLIENT_ID and SPOTIFY_CLIENT_SECRET environment variables." << std::endl;
    } else {
        // Start with loading state and automatically fetch data for hardcoded artist
        setLoadingState();
        std::cout << "🔍 Auto-fetching data for Jay Thakur..." << std::endl;
        refreshData();
    }
    
    rotatingText_->setRotationInterval(3000); // 3 seconds per text
    rotatingText_->start();
    
    isRunning_ = true;
    printStartupInfo();
    
    return true;
}

void SpotifyApp::update() {
    if (!isRunning_) {
        return;
    }
    
    // Update rotating text
    rotatingText_->update();
    
    // Update display with current rotating text
    display_->update(rotatingText_->getCurrentText());
}

void SpotifyApp::updateValue(int newValue) {
    // This method is deprecated - use setArtistId and refreshData instead
    std::cout << "Note: updateValue is deprecated. Use setArtistId() and refreshData() instead." << std::endl;
}

void SpotifyApp::cleanup() {
    if (display_) {
        delete display_;
        display_ = nullptr;
    }
    
    if (rotatingText_) {
        delete rotatingText_;
        rotatingText_ = nullptr;
    }
    
    if (spotifyAPI_) {
        delete spotifyAPI_;
        spotifyAPI_ = nullptr;
    }
    
    isRunning_ = false;
}

void SpotifyApp::setBrightness(int brightnessLevel) {
    if (brightnessLevel >= 1 && brightnessLevel <= 10) {
        brightnessLevel_ = brightnessLevel;
        if (display_) {
            display_->setBrightness(brightnessLevel);
        }
    }
}

void SpotifyApp::printStartupInfo() {
    std::cout << "\033[1;32m🎵 Spotify Counter Application\033[0m" << std::endl;
    std::cout << "\033[0;33m💡 Brightness:\033[0m " << brightnessLevel_ << "/10 (" << (brightnessLevel_ * 10) << "%)" << std::endl;
    std::cout << "\033[0;32m🎤 Artist:\033[0m Jay Thakur (auto-loaded)" << std::endl;
    std::cout << "\033[0;34m🔄 Press 'r' to refresh data\033[0m" << std::endl;
    std::cout << "\033[0;31m⚠️  Type 'back' to return to main menu\033[0m" << std::endl;
    std::cout << std::endl;
}

void SpotifyApp::setupMatrixOptions(RGBMatrix::Options& options, RuntimeOptions& runtimeOpt) {
    // This method is not used in this app since matrix is passed from main
}

void SpotifyApp::setArtistId(const std::string& artistId) {
    artistId_ = artistId;
    if (!artistId_.empty()) {
        refreshData();
    }
}

void SpotifyApp::refreshData() {
    if (artistId_.empty()) {
        setErrorState("No artist ID set");
        return;
    }
    
    if (!spotifyAPI_->isConfigured()) {
        setErrorState("API credentials not configured");
        return;
    }
    
    setLoadingState();
    
    // Make API call
    std::cout << "🔍 Fetching data for artist ID: " << artistId_ << std::endl;
    SpotifyArtistStats stats = spotifyAPI_->getArtistStats(artistId_);
    
    if (stats.isValid) {
        setDataState(stats);
        std::cout << "✅ Data refreshed successfully!" << std::endl;
    } else {
        setErrorState(stats.errorMessage);
        std::cerr << "❌ Error refreshing data: " << stats.errorMessage << std::endl;
    }
}

void SpotifyApp::handleKeyboardInput(char key) {
    switch (key) {
        case 'r':
        case 'R':
            if (!artistId_.empty()) {
                std::cout << "🔄 Refreshing data..." << std::endl;
                refreshData();
            } else {
                std::cout << "⚠️  Please set an artist ID first" << std::endl;
            }
            break;
        default:
            // Handle artist ID input
            if (key >= 32 && key <= 126) { // Printable characters
                if (artistId_.length() < 50) { // Reasonable limit
                    artistId_ += key;
                    std::cout << "Artist ID: " << artistId_ << std::endl;
                }
            } else if (key == '\n' || key == '\r') { // Enter key
                if (!artistId_.empty()) {
                    std::cout << "🔍 Fetching data for artist: " << artistId_ << std::endl;
                    refreshData();
                }
            } else if (key == '\b' || key == 127) { // Backspace
                if (!artistId_.empty()) {
                    artistId_.pop_back();
                    std::cout << "Artist ID: " << artistId_ << std::endl;
                }
            }
            break;
    }
}

void SpotifyApp::updateRotatingText() {
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
        if (!artistName_.empty()) {
            rotatingText_->addText(artistName_);
        }
        if (popularity_ > 0) {
            rotatingText_->addText("Popularity: " + std::to_string(popularity_));
        }
        if (albumCount_ > 0) {
            rotatingText_->addText(std::to_string(albumCount_) + " Albums");
        }
        if (!topTrack_.empty()) {
            rotatingText_->addText("Top: " + topTrack_);
        }
        if (!genres_.empty()) {
            rotatingText_->addText(genres_);
        }
    }
    
    rotatingText_->start();
}

void SpotifyApp::setLoadingState() {
    isLoading_ = true;
    hasError_ = false;
    errorMessage_.clear();
    updateRotatingText();
}

void SpotifyApp::setErrorState(const std::string& error) {
    isLoading_ = false;
    hasError_ = true;
    errorMessage_ = error;
    updateRotatingText();
}

void SpotifyApp::setDataState(const SpotifyArtistStats& stats) {
    isLoading_ = false;
    hasError_ = false;
    errorMessage_.clear();
    
    artistName_ = stats.name;
    popularity_ = stats.popularity;
    monthlyListeners_ = stats.monthlyListeners;
    albumCount_ = stats.albumCount;
    trackCount_ = stats.trackCount;
    topTrack_ = stats.topTrack;
    genres_ = stats.genres;
    
    updateRotatingText();
}

std::string SpotifyApp::formatNumber(int number) const {
    if (number >= 1000000) {
        return std::to_string(number / 1000000) + "M";
    } else if (number >= 1000) {
        return std::to_string(number / 1000) + "K";
    } else {
        return std::to_string(number);
    }
}
