#include "blink_manager.h"

BlinkManager::BlinkManager() : currentState_(true), previousState_(false) {
}

BlinkManager::~BlinkManager() {
    // No cleanup needed
}

bool BlinkManager::updateBlinkState() {
    previousState_ = currentState_;
    currentState_ = !currentState_;
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
