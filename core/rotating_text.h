#ifndef ROTATING_TEXT_H
#define ROTATING_TEXT_H

#include <vector>
#include <string>
#include <chrono>

class RotatingText {
public:
    RotatingText();
    ~RotatingText();
    
    // Configuration
    void addText(const std::string& text);
    void clearTexts();
    void setRotationInterval(int intervalMs);
    void setEnabled(bool enabled);
    
    // Control
    void start();
    void stop();
    void reset();
    
    // Getters
    std::string getCurrentText() const;
    bool isEnabled() const;
    bool hasTexts() const;
    
    // Update method (call this in your main loop)
    void update();
    
private:
    std::vector<std::string> texts_;
    size_t currentIndex_;
    int rotationIntervalMs_;
    bool enabled_;
    long long lastRotationTime_;
    
    void rotateToNext();
    long long getCurrentTimeMs() const;
};

#endif // ROTATING_TEXT_H
