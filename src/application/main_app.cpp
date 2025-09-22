#include "main_app.h"
#include "infrastructure/input/input_handler.h"
#include <iostream>
#include <signal.h>
#include <unistd.h>

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
    interrupt_received = true;
}

MainApp::MainApp(int argc, char** argv) 
    : matrix_(nullptr), argParser_(nullptr), inputHandler_(nullptr), 
      dbMeterApp_(nullptr), youtubeApp_(nullptr), spotifyApp_(nullptr),
      isRunning_(false), currentApp_(""), brightnessLevel_(5) {
    
    // Parse command line arguments
    argParser_ = new ArgParser(argc, argv);
    
    // Show help if requested
    if (argParser_->hasHelp()) {
        argParser_->printHelp(argv[0]);
        exit(0);
    }
    
    brightnessLevel_ = argParser_->getBrightness();
}

MainApp::~MainApp() {
    cleanup();
}

bool MainApp::initialize() {
    // Set up interrupt handler
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);
    
    // Create matrix options
    RGBMatrix::Options matrix_options;
    RuntimeOptions runtime_opt;
    setupMatrixOptions(matrix_options, runtime_opt);
    
    // Create matrix
    matrix_ = CreateMatrixFromOptions(matrix_options, runtime_opt);
    if (!matrix_) {
        std::cerr << "\033[0;31m❌ Could not initialize matrix\033[0m" << std::endl;
        return false;
    }
    
    // Create shared input handler
    inputHandler_ = new InputHandler();
    
    // Create feature apps (but don't initialize them yet)
    dbMeterApp_ = new DbMeterApp(matrix_, brightnessLevel_);
    youtubeApp_ = new YoutubeApp(matrix_, brightnessLevel_);
    spotifyApp_ = new SpotifyApp(matrix_, brightnessLevel_);
    
    isRunning_ = true;
    printMainMenu();
    
    return true;
}

void MainApp::run() {
    if (!isRunning_) {
        std::cerr << "\033[0;31m❌ Application not initialized\033[0m" << std::endl;
        return;
    }
    
    while (!interrupt_received && isRunning_) {
        // Check for new input
        if (inputHandler_->hasInput()) {
            if (currentApp_.empty()) {
                // No app active - treat as command
                std::string command = inputHandler_->readStringValue();
                
                if (command == "quit" || command == "exit" || command == "q") {
                    break;
                }
                
                handleCommand(command);
            } else {
                // App is active - check if it's a command first
                std::string input = inputHandler_->readStringValue();
                
                if (input == "back" || input == "b") {
                    // Return to main menu
                    currentApp_ = "";
                    std::cout << "\n\033[0;32m🔙 Returned to main menu\033[0m" << std::endl;
                    printMainMenu();
                } else {
                    // Handle input for active app
                    if (currentApp_ == "db") {
                        // Try to parse as numeric input for DB meter
                        try {
                            int newValue = std::stoi(input);
                            if (inputHandler_->isValidIntValue(newValue, 0, 120)) {
                                dbMeterApp_->updateValue(newValue);
                            }
                        } catch (const std::invalid_argument&) {
                            // Invalid input, ignore
                        }
                    } else if (currentApp_ == "youtube") {
                        // Handle YouTube app input (channel ID or commands)
                        if (input.length() == 1) {
                            // Single character input - handle as keyboard input
                            youtubeApp_->handleKeyboardInput(input[0]);
                        } else {
                            // Multi-character input - treat as channel ID
                            youtubeApp_->setChannelId(input);
                        }
                    } else if (currentApp_ == "spotify") {
                        // Handle Spotify app input (artist ID or commands)
                        if (input.length() == 1) {
                            // Single character input - handle as keyboard input
                            spotifyApp_->handleKeyboardInput(input[0]);
                        } else {
                            // Multi-character input - treat as artist ID
                            spotifyApp_->setArtistId(input);
                        }
                    }
                }
            }
        }
        
        // Update current app display
        if (currentApp_ == "db") {
            dbMeterApp_->update();
        } else if (currentApp_ == "youtube") {
            youtubeApp_->update();
        } else if (currentApp_ == "spotify") {
            spotifyApp_->update();
        }
        
        // Small delay
        usleep(10000); // 10ms
    }
    
    std::cout << "\n\033[0;31m⚠️  Received CTRL-C. Exiting.\033[0m" << std::endl;
}

void MainApp::cleanup() {
    cleanupCurrentApp();
    
    if (inputHandler_) {
        delete inputHandler_;
        inputHandler_ = nullptr;
    }
    
    if (dbMeterApp_) {
        delete dbMeterApp_;
        dbMeterApp_ = nullptr;
    }
    
    if (youtubeApp_) {
        delete youtubeApp_;
        youtubeApp_ = nullptr;
    }
    
    if (spotifyApp_) {
        delete spotifyApp_;
        spotifyApp_ = nullptr;
    }
    
    if (argParser_) {
        delete argParser_;
        argParser_ = nullptr;
    }
    
    if (matrix_) {
        matrix_->Clear();
        delete matrix_;
        matrix_ = nullptr;
    }
    
    isRunning_ = false;
}

void MainApp::setupMatrixOptions(RGBMatrix::Options& options, RuntimeOptions& runtime_opt) {
    // Configure matrix to match your working setup
    options.rows = 48;                    // --led-rows=48
    options.cols = 96;                    // --led-cols=96
    options.chain_length = 1;
    options.parallel = 1;
    options.hardware_mapping = "regular";
    options.brightness = brightnessLevel_ * 10; // Convert 1-10 to 10-100
    options.disable_hardware_pulsing = true;   // --led-no-hardware-pulse
    options.pwm_bits = 11;                // --led-pwm-bits=11
    options.pwm_lsb_nanoseconds = 110;    // --led-pwm-lsb-nanoseconds 130
    
    runtime_opt.gpio_slowdown = 4;        // --led-slowdown-gpio=4
}

void MainApp::printMainMenu() {
    std::cout << "\033[1;36m🎮 LED Matrix Applications\033[0m" << std::endl;
    std::cout << "\033[0;33m💡 Brightness:\033[0m " << brightnessLevel_ << "/10 (" << (brightnessLevel_ * 10) << "%)" << std::endl;
    std::cout << std::endl;
    std::cout << "\033[0;32m📋 Available Apps:\033[0m" << std::endl;
    std::cout << "  \033[0;34mdb\033[0m        - dB Level Meter" << std::endl;
    std::cout << "  \033[0;34myoutube\033[0m   - YouTube Subscriber Counter" << std::endl;
    std::cout << "  \033[0;34mspotify\033[0m   - Spotify Artist Statistics" << std::endl;
    std::cout << "  \033[0;34mquit\033[0m      - Exit application" << std::endl;
    std::cout << std::endl;
    std::cout << "\033[0;32m💡 Type an app name to switch to it:\033[0m" << std::endl;
}

void MainApp::handleCommand(const std::string& command) {
    if (command == "db") {
        switchToApp("db");
    } else if (command == "youtube" || command == "yt") {
        switchToApp("youtube");
    } else if (command == "spotify" || command == "sp") {
        switchToApp("spotify");
    } else if (command == "back" || command == "menu") {
        cleanupCurrentApp();
        currentApp_ = "";
        printMainMenu();
    } else {
        std::cout << "\033[0;31m❌ Unknown command: " << command << "\033[0m" << std::endl;
        std::cout << "\033[0;32m💡 Available commands: db, youtube, spotify, back, quit\033[0m" << std::endl;
    }
}

void MainApp::switchToApp(const std::string& appName) {
    cleanupCurrentApp();
    currentApp_ = appName;
    
    if (appName == "db") {
        std::cout << "\033[1;36m🎵 Switching to dB Meter...\033[0m" << std::endl;
        if (!dbMeterApp_->initialize()) {
            std::cerr << "\033[0;31m❌ Failed to initialize dB Meter app\033[0m" << std::endl;
            currentApp_ = "";
        }
    } else if (appName == "youtube") {
        std::cout << "\033[1;36m📺 Switching to YouTube Counter...\033[0m" << std::endl;
        if (!youtubeApp_->initialize()) {
            std::cerr << "\033[0;31m❌ Failed to initialize YouTube Counter app\033[0m" << std::endl;
            currentApp_ = "";
        }
    } else if (appName == "spotify") {
        std::cout << "\033[1;36m🎵 Switching to Spotify Counter...\033[0m" << std::endl;
        if (!spotifyApp_->initialize()) {
            std::cerr << "\033[0;31m❌ Failed to initialize Spotify Counter app\033[0m" << std::endl;
            currentApp_ = "";
        }
    } else {
        std::cout << "\033[0;31m❌ Unknown app: " << appName << "\033[0m" << std::endl;
    }
}

void MainApp::cleanupCurrentApp() {
    if (currentApp_ == "db") {
        // Cleanup dB meter app if needed
    } else if (currentApp_ == "youtube") {
        // Cleanup YouTube app if needed
    } else if (currentApp_ == "spotify") {
        // Cleanup Spotify app if needed
    }
}
