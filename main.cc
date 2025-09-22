#include "main_app.h"
#include <iostream>

int main(int argc, char **argv) {
    // Create and initialize main application
    MainApp app(argc, argv);
    
    if (!app.initialize()) {
        std::cerr << "\033[0;31m❌ Failed to initialize application\033[0m" << std::endl;
        return 1;
    }
    
    // Run the application
    app.run();
    app.cleanup();
    
    return 0;
}
