#ifndef NETWORK_HANDLER_H
#define NETWORK_HANDLER_H

#include <string>
#include <map>

class NetworkHandler {
public:
    NetworkHandler();
    ~NetworkHandler();
    
    // HTTP request methods
    bool get(const std::string& url, std::string& response);
    bool get(const std::string& url, const std::map<std::string, std::string>& headers, std::string& response);
    bool post(const std::string& url, const std::map<std::string, std::string>& headers, const std::string& data, std::string& response);
    
    // Utility methods
    void setTimeout(int timeoutSeconds);
    void setUserAgent(const std::string& userAgent);
    
    // Error handling
    std::string getLastError() const;
    int getLastHttpCode() const;
    
private:
    class Impl;
    Impl* impl_;
    
    // Disable copy constructor and assignment operator
    NetworkHandler(const NetworkHandler&) = delete;
    NetworkHandler& operator=(const NetworkHandler&) = delete;
};

#endif // NETWORK_HANDLER_H
