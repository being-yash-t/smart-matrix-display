#include "spotify_api.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <cstdlib>

// Simple base64 encoding function
std::string base64_encode(const std::string& input) {
    const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    int val = 0, valb = -6;
    
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    
    if (valb > -6) {
        result.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    
    while (result.size() % 4) {
        result.push_back('=');
    }
    
    return result;
}

SpotifyAPI::SpotifyAPI() {
    // Get credentials from environment variables
    const char* clientId = std::getenv("SPOTIFY_CLIENT_ID");
    const char* clientSecret = std::getenv("SPOTIFY_CLIENT_SECRET");
    
    if (clientId && clientSecret) {
        clientId_ = std::string(clientId);
        clientSecret_ = std::string(clientSecret);
    } else {
        lastError_ = "SPOTIFY_CLIENT_ID and SPOTIFY_CLIENT_SECRET environment variables not set";
        std::cerr << "Warning: " << lastError_ << std::endl;
    }
}

SpotifyAPI::~SpotifyAPI() = default;

void SpotifyAPI::setClientCredentials(const std::string& clientId, const std::string& clientSecret) {
    clientId_ = clientId;
    clientSecret_ = clientSecret;
    accessToken_.clear(); // Clear any existing token
    lastError_.clear();
}

bool SpotifyAPI::isConfigured() const {
    return !clientId_.empty() && !clientSecret_.empty();
}

std::string SpotifyAPI::getLastError() const {
    return lastError_;
}

bool SpotifyAPI::authenticate() {
    if (!isConfigured()) {
        lastError_ = "Client credentials not configured";
        return false;
    }
    
    // Build authentication URL
    std::string authUrl = buildAuthUrl();
    
    // Prepare headers for authentication
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/x-www-form-urlencoded";
    
    // Create basic auth header
    std::string credentials = clientId_ + ":" + clientSecret_;
    std::string encodedCredentials = base64_encode(credentials);
    headers["Authorization"] = "Basic " + encodedCredentials;
    
    // Make authentication request
    std::string response;
    std::cout << "🔐 Authenticating with Spotify..." << std::endl;
    std::cout << "🔗 Auth URL: " << authUrl << std::endl;
    
    if (!networkHandler_.post(authUrl, headers, "grant_type=client_credentials", response)) {
        lastError_ = "Authentication failed: " + networkHandler_.getLastError();
        std::cerr << "❌ Auth HTTP Error: " << networkHandler_.getLastHttpCode() << std::endl;
        std::cerr << "❌ Auth Response: " << response << std::endl;
        return false;
    }
    
    std::cout << "✅ Auth response received" << std::endl;
    
    // Extract access token from response
    accessToken_ = extractJsonValue(response, "access_token");
    if (accessToken_.empty()) {
        lastError_ = "Failed to extract access token from response";
        std::cerr << "❌ Auth response: " << response << std::endl;
        return false;
    }
    
    std::cout << "✅ Access token obtained" << std::endl;
    
    return true;
}

SpotifyArtistStats SpotifyAPI::getArtistStats(const std::string& artistId) {
    SpotifyArtistStats stats;
    lastError_.clear();
    
    if (!isConfigured()) {
        stats.errorMessage = "Client credentials not configured";
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    if (artistId.empty()) {
        stats.errorMessage = "Artist ID cannot be empty";
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    // Authenticate if we don't have a token
    if (accessToken_.empty()) {
        if (!authenticate()) {
            stats.errorMessage = lastError_;
            return stats;
        }
    }
    
    // Get artist basic info
    std::string artistUrl = buildArtistUrl(artistId);
    std::string artistResponse;
    
    std::map<std::string, std::string> headers;
    headers["Authorization"] = "Bearer " + accessToken_;
    
    // Debug output
    std::cout << "🔍 Making request to: " << artistUrl << std::endl;
    std::cout << "🔑 Using access token: " << accessToken_.substr(0, 20) << "..." << std::endl;
    
    if (!networkHandler_.get(artistUrl, headers, artistResponse)) {
        stats.errorMessage = "Network error getting artist info: " + networkHandler_.getLastError();
        lastError_ = stats.errorMessage;
        std::cerr << "❌ HTTP Error: " << networkHandler_.getLastHttpCode() << std::endl;
        std::cerr << "❌ Response: " << artistResponse << std::endl;
        return stats;
    }
    
    // Parse artist response
    stats = parseArtistResponse(artistResponse);
    if (!stats.isValid) {
        lastError_ = stats.errorMessage;
        return stats;
    }
    
    // Get artist albums for album count
    std::string albumsUrl = buildArtistAlbumsUrl(artistId);
    std::string albumsResponse;
    
    if (networkHandler_.get(albumsUrl, headers, albumsResponse)) {
        stats = parseAlbumsResponse(albumsResponse, stats);
    }
    
    // Get top tracks for top track info
    std::string topTracksUrl = buildArtistTopTracksUrl(artistId);
    std::string topTracksResponse;
    
    if (networkHandler_.get(topTracksUrl, headers, topTracksResponse)) {
        stats = parseTopTracksResponse(topTracksResponse, stats);
    }
    
    return stats;
}

std::string SpotifyAPI::buildAuthUrl() {
    return "https://accounts.spotify.com/api/token";
}

std::string SpotifyAPI::buildArtistUrl(const std::string& artistId) {
    std::stringstream url;
    url << "https://api.spotify.com/v1/artists/" << artistId;
    return url.str();
}

std::string SpotifyAPI::buildArtistAlbumsUrl(const std::string& artistId) {
    std::stringstream url;
    url << "https://api.spotify.com/v1/artists/" << artistId << "/albums?limit=50";
    return url.str();
}

std::string SpotifyAPI::buildArtistTopTracksUrl(const std::string& artistId) {
    std::stringstream url;
    url << "https://api.spotify.com/v1/artists/" << artistId << "/top-tracks?market=US";
    return url.str();
}

SpotifyArtistStats SpotifyAPI::parseArtistResponse(const std::string& jsonResponse) {
    SpotifyArtistStats stats;
    
    // Check for errors
    if (jsonResponse.find("\"error\"") != std::string::npos) {
        stats.errorMessage = "API error in response";
        return stats;
    }
    
    // Extract basic artist info
    stats.name = extractJsonValue(jsonResponse, "name");
    stats.popularity = parseIntValue(extractJsonValue(jsonResponse, "popularity"));
    stats.genres = formatGenres(extractJsonValue(jsonResponse, "genres"));
    
    if (stats.name.empty()) {
        stats.errorMessage = "No artist data found in response";
        return stats;
    }
    
    stats.isValid = true;
    return stats;
}

SpotifyArtistStats SpotifyAPI::parseAlbumsResponse(const std::string& jsonResponse, SpotifyArtistStats& stats) {
    // Count albums in the response
    std::regex albumRegex("\"album_type\"\\s*:\\s*\"(album|single|compilation)\"");
    std::sregex_iterator begin(jsonResponse.begin(), jsonResponse.end(), albumRegex);
    std::sregex_iterator end;
    
    stats.albumCount = std::distance(begin, end);
    
    // Count total tracks across all albums
    std::regex trackRegex("\"total_tracks\"\\s*:\\s*(\\d+)");
    std::sregex_iterator trackBegin(jsonResponse.begin(), jsonResponse.end(), trackRegex);
    std::sregex_iterator trackEnd;
    
    stats.trackCount = 0;
    for (auto it = trackBegin; it != trackEnd; ++it) {
        stats.trackCount += parseIntValue((*it)[1].str());
    }
    
    return stats;
}

SpotifyArtistStats SpotifyAPI::parseTopTracksResponse(const std::string& jsonResponse, SpotifyArtistStats& stats) {
    // Extract the first (most popular) track name
    stats.topTrack = extractJsonArrayValue(jsonResponse, "name", 0);
    return stats;
}

std::string SpotifyAPI::extractJsonValue(const std::string& json, const std::string& key) {
    std::string pattern = "\"" + key + "\"\\s*:\\s*\"([^\"]+)\"";
    std::regex regex(pattern);
    std::smatch match;
    
    if (std::regex_search(json, match, regex)) {
        return match[1].str();
    }
    
    return "";
}

std::string SpotifyAPI::extractJsonArrayValue(const std::string& json, const std::string& key, int index) {
    std::string pattern = "\"" + key + "\"\\s*:\\s*\"([^\"]+)\"";
    std::regex regex(pattern);
    std::sregex_iterator begin(json.begin(), json.end(), regex);
    std::sregex_iterator end;
    
    int count = 0;
    for (auto it = begin; it != end && count <= index; ++it, ++count) {
        if (count == index) {
            return (*it)[1].str();
        }
    }
    
    return "";
}

int SpotifyAPI::parseIntValue(const std::string& value) {
    if (value.empty()) {
        return 0;
    }
    
    try {
        return std::stoi(value);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing integer value '" << value << "': " << e.what() << std::endl;
        return 0;
    }
}

std::string SpotifyAPI::formatGenres(const std::string& genresJson) {
    // This is a simplified version - in reality, genres come as an array
    // For now, we'll just return the first genre or a default
    if (genresJson.empty()) {
        return "Electronic";
    }
    return genresJson;
}
