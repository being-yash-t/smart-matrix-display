#ifndef SPOTIFY_APP_H
#define SPOTIFY_APP_H

#include "presentation/displays/spotify_display.h"
#include "infrastructure/input/input_handler.h"
#include "shared/utils/rotating_text.h"
#include "infrastructure/network/spotify_api.h"
#include <string>

class SpotifyApp {
public:
    SpotifyApp(RGBMatrix* matrix, int brightnessLevel = 5);
    ~SpotifyApp();
    
    // Application lifecycle
    bool initialize();
    void update();
    void updateValue(int newValue);
    void cleanup();
    
    // App control
    void setBrightness(int brightnessLevel);
    void printStartupInfo();
    
    // API integration
    void setArtistId(const std::string& artistId);
    void refreshData();
    void handleKeyboardInput(char key);
    
private:
    // Components
    RGBMatrix* matrix_;
    SpotifyDisplay* display_;
    
    // State
    bool isRunning_;
    std::string artistName_;
    int popularity_;
    int monthlyListeners_;
    int albumCount_;
    int trackCount_;
    std::string topTrack_;
    std::string genres_;
    int brightnessLevel_;
    
    // API and data state
    SpotifyAPI* spotifyAPI_;
    std::string artistId_;
    bool isLoading_;
    bool hasError_;
    std::string errorMessage_;
    
    // Components
    RotatingText* rotatingText_;
    
    // Helper methods
    void setupMatrixOptions(RGBMatrix::Options& options, RuntimeOptions& runtimeOpt);
    void updateRotatingText();
    void setLoadingState();
    void setErrorState(const std::string& error);
    void setDataState(const SpotifyArtistStats& stats);
    std::string formatNumber(int number) const;
};

#endif // SPOTIFY_APP_H
