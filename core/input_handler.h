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
    
    // Read dB value from stdin
    int readDbValue();
    
    // Validate dB value
    bool isValidDbValue(int value);
    
private:
    fd_set readfds_;
    struct timeval timeout_;
};

#endif // INPUT_HANDLER_H
