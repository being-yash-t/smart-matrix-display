#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <sys/select.h>
#include <string>

class InputHandler {
public:
    InputHandler();
    ~InputHandler();
    
    // Check for new input (non-blocking)
    bool hasInput();
    
    // Read integer value from stdin
    int readIntValue();
    
    // Read string value from stdin
    std::string readStringValue();
    
    // Validate integer value within range
    bool isValidIntValue(int value, int min = 0, int max = 100);
    
private:
    fd_set readfds_;
    struct timeval timeout_;
};

#endif // INPUT_HANDLER_H
