#ifndef SPOTIFY_API_H
#define SPOTIFY_API_H

#include "shared/network/network_handler.h"
#include <string>

struct SpotifyArtistStats {
    std::string name;
    int popularity;
    int monthlyListeners;
    int albumCount;
    int trackCount;
    std::string topTrack;
    std::string genres;
    bool isValid;
    std::string errorMessage;
    
    SpotifyArtistStats() : popularity(0), monthlyListeners(0), albumCount(0), trackCount(0), isValid(false) {}
};

class SpotifyAPI {
public:
    SpotifyAPI();
    ~SpotifyAPI();
    
    // Main API methods
    SpotifyArtistStats getArtistStats(const std::string& artistId);
    
    // Configuration
    void setClientCredentials(const std::string& clientId, const std::string& clientSecret);
    bool isConfigured() const;
    
    // Error handling
    std::string getLastError() const;
    
private:
    NetworkHandler networkHandler_;
    std::string clientId_;
    std::string clientSecret_;
    std::string accessToken_;
    std::string lastError_;
    
    // Helper methods
    bool authenticate();
    std::string buildAuthUrl();
    std::string buildArtistUrl(const std::string& artistId);
    std::string buildArtistAlbumsUrl(const std::string& artistId);
    std::string buildArtistTopTracksUrl(const std::string& artistId);
    SpotifyArtistStats parseArtistResponse(const std::string& jsonResponse);
    SpotifyArtistStats parseAlbumsResponse(const std::string& jsonResponse, SpotifyArtistStats& stats);
    SpotifyArtistStats parseTopTracksResponse(const std::string& jsonResponse, SpotifyArtistStats& stats);
    std::string extractJsonValue(const std::string& json, const std::string& key);
    std::string extractJsonArrayValue(const std::string& json, const std::string& key, int index = 0);
    int parseIntValue(const std::string& value);
    std::string formatGenres(const std::string& genresJson);
    
    // Disable copy constructor and assignment operator
    SpotifyAPI(const SpotifyAPI&) = delete;
    SpotifyAPI& operator=(const SpotifyAPI&) = delete;
};

#endif // SPOTIFY_API_H
