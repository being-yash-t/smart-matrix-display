#ifndef YOUTUBE_API_H
#define YOUTUBE_API_H

#include "shared/network/network_handler.h"
#include <string>

struct YouTubeChannelStats {
    long subscriberCount;
    long viewCount;
    long videoCount;
    bool isValid;
    std::string errorMessage;
    
    YouTubeChannelStats() : subscriberCount(0), viewCount(0), videoCount(0), isValid(false) {}
};

class YouTubeAPI {
public:
    YouTubeAPI();
    ~YouTubeAPI();
    
    // Main API methods
    YouTubeChannelStats getChannelStats(const std::string& channelId);
    YouTubeChannelStats getChannelStatsByUsername(const std::string& username);
    
    // Configuration
    void setApiKey(const std::string& apiKey);
    bool isConfigured() const;
    
    // Error handling
    std::string getLastError() const;
    
private:
    NetworkHandler networkHandler_;
    std::string apiKey_;
    std::string lastError_;
    
    // Helper methods
    std::string buildChannelStatsUrl(const std::string& channelId);
    std::string buildChannelSearchUrl(const std::string& username);
    YouTubeChannelStats parseChannelStatsResponse(const std::string& jsonResponse);
    std::string extractChannelIdFromSearch(const std::string& jsonResponse);
    std::string extractJsonValue(const std::string& json, const std::string& key);
    long parseLongValue(const std::string& value);
    
    // Disable copy constructor and assignment operator
    YouTubeAPI(const YouTubeAPI&) = delete;
    YouTubeAPI& operator=(const YouTubeAPI&) = delete;
};

#endif // YOUTUBE_API_H
