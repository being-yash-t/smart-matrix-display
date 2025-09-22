#include "text_demo_app.h"
#include "../../core/arg_parser.h"
#include <iostream>

int main(int argc, char **argv) {
    // Parse command line arguments
    ArgParser parser(argc, argv);
    
    // Show help if requested
    if (parser.hasHelp()) {
        parser.printHelp(argv[0]);
        return 0;
    }
    
    // Create and initialize application with specified brightness
    TextDemoApp app(parser.getBrightness());
    
    if (!app.initialize()) {
        std::cerr << "\033[0;31m❌ Failed to initialize application\033[0m" << std::endl;
        return 1;
    }
    
    // Run the application
    app.run();
    app.cleanup();
    
    return 0;
}
