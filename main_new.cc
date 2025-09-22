#include "db_meter_app.h"
#include <iostream>

int main(int argc, char **argv) {
    DbMeterApp app;
    
    if (!app.initialize()) {
        std::cerr << "Failed to initialize application\n";
        return 1;
    }
    
    app.run();
    app.cleanup();
    
    return 0;
}
