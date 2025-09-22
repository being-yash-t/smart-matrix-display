#ifndef BLINK_MANAGER_H
#define BLINK_MANAGER_H

class BlinkManager {
public:
    BlinkManager();
    ~BlinkManager();
    
    // Update blink state and return current state
    bool updateBlinkState();
    
    // Get current blink state
    bool getCurrentState() const;
    
    // Get previous blink state
    bool getPreviousState() const;
    
    // Reset blink state
    void reset();
    
private:
    bool currentState_;
    bool previousState_;
};

#endif // BLINK_MANAGER_H
