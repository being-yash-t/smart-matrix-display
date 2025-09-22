#include "rotating_text.h"
#include <chrono>

RotatingText::RotatingText() 
    : currentIndex_(0), rotationIntervalMs_(3000), enabled_(false), lastRotationTime_(0) {
}

RotatingText::~RotatingText() {
    // No cleanup needed
}

void RotatingText::addText(const std::string& text) {
    texts_.push_back(text);
}

void RotatingText::setRotationInterval(int intervalMs) {
    rotationIntervalMs_ = intervalMs;
}

void RotatingText::setEnabled(bool enabled) {
    enabled_ = enabled;
    if (enabled && !texts_.empty()) {
        lastRotationTime_ = getCurrentTimeMs();
    }
}

void RotatingText::start() {
    if (!texts_.empty()) {
        enabled_ = true;
        lastRotationTime_ = getCurrentTimeMs();
    }
}

void RotatingText::stop() {
    enabled_ = false;
}

void RotatingText::reset() {
    currentIndex_ = 0;
    lastRotationTime_ = getCurrentTimeMs();
}

std::string RotatingText::getCurrentText() const {
    if (texts_.empty()) {
        return "";
    }
    return texts_[currentIndex_];
}

bool RotatingText::isEnabled() const {
    return enabled_;
}

bool RotatingText::hasTexts() const {
    return !texts_.empty();
}

void RotatingText::update() {
    if (!enabled_ || texts_.empty() || texts_.size() == 1) {
        return;
    }
    
    long long currentTime = getCurrentTimeMs();
    if (currentTime - lastRotationTime_ >= rotationIntervalMs_) {
        rotateToNext();
        lastRotationTime_ = currentTime;
    }
}

void RotatingText::rotateToNext() {
    currentIndex_ = (currentIndex_ + 1) % texts_.size();
}

long long RotatingText::getCurrentTimeMs() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}
