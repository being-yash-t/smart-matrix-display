#ifndef YOUTUBE_APP_H
#define YOUTUBE_APP_H

#include "../../display/youtube_display.h"
#include "../../core/input_handler.h"
#include "../../core/rotating_text.h"
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
    
private:
    // Components
    RGBMatrix* matrix_;
    YoutubeDisplay* display_;
    
    // State
    bool isRunning_;
    int currentSubscriberCount_;
    int currentPlays_;
    int currentLikes_;
    int brightnessLevel_;
    
    // Components
    RotatingText* rotatingText_;
    
    // Helper methods
    void setupMatrixOptions(RGBMatrix::Options& options, RuntimeOptions& runtimeOpt);
};

#endif // YOUTUBE_APP_H
