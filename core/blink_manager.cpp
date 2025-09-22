#include "blink_manager.h"
#include <chrono>
#include <ctime>

BlinkManager::BlinkManager() : currentState_(true), previousState_(false), lastToggleTime_(0) {
}

BlinkManager::~BlinkManager() {
    // No cleanup needed
}

bool BlinkManager::updateBlinkState(int durationMs) {
    // Get current time in milliseconds
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    
    // Toggle state based on provided duration
    if (now - lastToggleTime_ >= durationMs) {
        previousState_ = currentState_;
        currentState_ = !currentState_;
        lastToggleTime_ = now;
    }
    
    return currentState_;
}

bool BlinkManager::getCurrentState() const {
    return currentState_;
}

bool BlinkManager::getPreviousState() const {
    return previousState_;
}

void BlinkManager::reset() {
    currentState_ = true;
    previousState_ = false;
}
