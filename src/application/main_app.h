#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "led-matrix.h"
#include "graphics.h"
#include "infrastructure/config/arg_parser.h"
#include "infrastructure/input/input_handler.h"
#include "presentation/controllers/db_meter_app.h"
#include "presentation/controllers/youtube_app.h"
#include "presentation/controllers/spotify_app.h"
#include <string>

using namespace rgb_matrix;

class MainApp {
public:
    MainApp(int argc, char** argv);
    ~MainApp();
    
    // Application lifecycle
    bool initialize();
    void run();
    void cleanup();
    
private:
    // Components
    RGBMatrix* matrix_;
    ArgParser* argParser_;
    InputHandler* inputHandler_;
    
    // Feature apps
    DbMeterApp* dbMeterApp_;
    YoutubeApp* youtubeApp_;
    SpotifyApp* spotifyApp_;
    
    // State
    bool isRunning_;
    std::string currentApp_;
    int brightnessLevel_;
    
    // Helper methods
    void setupMatrixOptions(RGBMatrix::Options& options, RuntimeOptions& runtimeOpt);
    void printMainMenu();
    void handleCommand(const std::string& command);
    void switchToApp(const std::string& appName);
    void cleanupCurrentApp();
};

#endif // MAIN_APP_H
