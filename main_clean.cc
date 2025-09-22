#include "db_meter_app.h"
#include "arg_parser.h"
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
    DbMeterApp app(parser.getBrightness());
    
    if (!app.initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return 1;
    }
    
    // Run the application
    app.run();
    app.cleanup();
    
    return 0;
}
