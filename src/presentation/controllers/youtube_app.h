#ifndef YOUTUBE_APP_H
#define YOUTUBE_APP_H

#include "presentation/displays/youtube_display.h"
#include "infrastructure/input/input_handler.h"
#include "shared/utils/rotating_text.h"
#include "infrastructure/network/youtube_api.h"
#include <string>

class YoutubeApp {
public:
    YoutubeApp(RGBMatrix* matrix, int brightnessLevel = 5);
    ~YoutubeApp();
    
    // Application lifecycle
    bool initialize();
    void update();
    void updateValue(int newValue);
    void cleanup();
    
    // App control
    void setBrightness(int brightnessLevel);
    void printStartupInfo();
    
    // API integration
    void setChannelId(const std::string& channelId);
    void refreshData();
    void handleKeyboardInput(char key);
    
private:
    // Components
    RGBMatrix* matrix_;
    YoutubeDisplay* display_;
    
    // State
    bool isRunning_;
    long currentSubscriberCount_;
    long currentViewCount_;
    long currentVideoCount_;
    int brightnessLevel_;
    
    // API and data state
    YouTubeAPI* youtubeAPI_;
    std::string channelId_;
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
    void setDataState(const YouTubeChannelStats& stats);
    std::string formatNumber(long number) const;
};

#endif // YOUTUBE_APP_H
