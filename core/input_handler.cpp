#include "input_handler.h"
#include <iostream>
#include <unistd.h>

InputHandler::InputHandler() {
    FD_ZERO(&readfds_);
    FD_SET(STDIN_FILENO, &readfds_);
    timeout_.tv_sec = 0;
    timeout_.tv_usec = 0;
}

InputHandler::~InputHandler() {
    // No cleanup needed
}

bool InputHandler::hasInput() {
    fd_set temp_readfds = readfds_;
    struct timeval temp_timeout = timeout_;
    
    return select(STDIN_FILENO + 1, &temp_readfds, NULL, NULL, &temp_timeout) > 0;
}

int InputHandler::readIntValue() {
    int value = -1;
    std::string line;
    std::getline(std::cin, line);
    try { 
        value = std::stoi(line); 
    }
    catch(...) { 
        value = -1; 
    }
    return value;
}

std::string InputHandler::readStringValue() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

bool InputHandler::isValidIntValue(int value, int min, int max) {
    return value >= min && value <= max;
}
