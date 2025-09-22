#include "youtube_api.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <cstdlib>

YouTubeAPI::YouTubeAPI() {
    // Get API key from environment variable
    const char* apiKey = std::getenv("YOUTUBE_API_KEY");
    if (apiKey) {
        apiKey_ = std::string(apiKey);
    } else {
        lastError_ = "YOUTUBE_API_KEY environment variable not set";
        std::cerr << "Warning: " << lastError_ << std::endl;
    }
}

YouTubeAPI::~YouTubeAPI() = default;

void YouTubeAPI::setApiKey(const std::string& apiKey) {
    apiKey_ = apiKey;
    lastError_.clear();
}

bool YouTubeAPI::isConfigured() const {
    return !apiKey_.empty();
}

std::string YouTubeAPI::getLastError() const {
    return lastError_;
}

YouTubeChannelStats YouTubeAPI::getChannelStats(const std::string& channelId) {
    YouTubeChannelStats stats;
    lastError_.clear();
    
    if (!isConfigured()) {
        stats.errorMessage = "API key not configured";
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    if (channelId.empty()) {
        stats.errorMessage = "Channel ID cannot be empty";
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    std::string url = buildChannelStatsUrl(channelId);
    std::string response;
    
    if (!networkHandler_.get(url, response)) {
        stats.errorMessage = "Network error: " + networkHandler_.getLastError();
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    // Check for API errors in response
    if (response.find("\"error\"") != std::string::npos) {
        if (response.find("API_KEY_INVALID") != std::string::npos) {
            stats.errorMessage = "Invalid API key";
        } else if (response.find("QUOTA_EXCEEDED") != std::string::npos) {
            stats.errorMessage = "API quota exceeded";
        } else if (response.find("CHANNEL_NOT_FOUND") != std::string::npos) {
            stats.errorMessage = "Channel not found";
        } else {
            stats.errorMessage = "API error";
        }
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    stats = parseChannelStatsResponse(response);
    if (!stats.isValid) {
        lastError_ = stats.errorMessage;
    }
    
    return stats;
}

YouTubeChannelStats YouTubeAPI::getChannelStatsByUsername(const std::string& username) {
    YouTubeChannelStats stats;
    lastError_.clear();
    
    if (!isConfigured()) {
        stats.errorMessage = "API key not configured";
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    if (username.empty()) {
        stats.errorMessage = "Username cannot be empty";
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    // First, search for the channel using the username
    std::string searchUrl = buildChannelSearchUrl(username);
    std::string searchResponse;
    
    if (!networkHandler_.get(searchUrl, searchResponse)) {
        stats.errorMessage = "Network error searching for channel: " + networkHandler_.getLastError();
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    // Extract channel ID from search response
    std::string channelId = extractChannelIdFromSearch(searchResponse);
    if (channelId.empty()) {
        stats.errorMessage = "Could not find channel ID for username: " + username;
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    // Now get the channel stats using the channel ID
    return getChannelStats(channelId);
}

std::string YouTubeAPI::buildChannelStatsUrl(const std::string& channelId) {
    std::stringstream url;
    url << "https://www.googleapis.com/youtube/v3/channels?"
        << "part=statistics&"
        << "id=" << channelId << "&"
        << "key=" << apiKey_;
    return url.str();
}

std::string YouTubeAPI::buildChannelSearchUrl(const std::string& username) {
    std::stringstream url;
    url << "https://www.googleapis.com/youtube/v3/search?"
        << "part=snippet&"
        << "type=channel&"
        << "q=" << username << "&"
        << "key=" << apiKey_;
    return url.str();
}

YouTubeChannelStats YouTubeAPI::parseChannelStatsResponse(const std::string& jsonResponse) {
    YouTubeChannelStats stats;
    
    // Check if we have items in the response
    if (jsonResponse.find("\"items\"") == std::string::npos || 
        jsonResponse.find("\"items\":[]") != std::string::npos) {
        stats.errorMessage = "No channel data found";
        return stats;
    }
    
    // Extract statistics
    std::string subscriberCountStr = extractJsonValue(jsonResponse, "subscriberCount");
    std::string viewCountStr = extractJsonValue(jsonResponse, "viewCount");
    std::string videoCountStr = extractJsonValue(jsonResponse, "videoCount");
    
    if (subscriberCountStr.empty() && viewCountStr.empty() && videoCountStr.empty()) {
        stats.errorMessage = "No statistics found in response";
        return stats;
    }
    
    stats.subscriberCount = parseLongValue(subscriberCountStr);
    stats.viewCount = parseLongValue(viewCountStr);
    stats.videoCount = parseLongValue(videoCountStr);
    stats.isValid = true;
    
    return stats;
}

std::string YouTubeAPI::extractChannelIdFromSearch(const std::string& jsonResponse) {
    // Look for the first channel in the search results
    std::string pattern = "\"channelId\"\\s*:\\s*\"([^\"]+)\"";
    std::regex regex(pattern);
    std::smatch match;
    
    if (std::regex_search(jsonResponse, match, regex)) {
        return match[1].str();
    }
    
    return "";
}

std::string YouTubeAPI::extractJsonValue(const std::string& json, const std::string& key) {
    std::string pattern = "\"" + key + "\"\\s*:\\s*\"([^\"]+)\"";
    std::regex regex(pattern);
    std::smatch match;
    
    if (std::regex_search(json, match, regex)) {
        return match[1].str();
    }
    
    // Try without quotes (for numbers)
    pattern = "\"" + key + "\"\\s*:\\s*([0-9]+)";
    regex = std::regex(pattern);
    
    if (std::regex_search(json, match, regex)) {
        return match[1].str();
    }
    
    return "";
}

long YouTubeAPI::parseLongValue(const std::string& value) {
    if (value.empty()) {
        return 0;
    }
    
    try {
        return std::stol(value);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing value '" << value << "': " << e.what() << std::endl;
        return 0;
    }
}
