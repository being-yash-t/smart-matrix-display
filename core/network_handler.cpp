#include "network_handler.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>

class NetworkHandler::Impl {
public:
    CURL* curl_;
    std::string lastError_;
    int lastHttpCode_;
    int timeoutSeconds_;
    std::string userAgent_;
    
    Impl() : curl_(nullptr), lastHttpCode_(0), timeoutSeconds_(30), userAgent_("StatsBoards/1.0") {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl_ = curl_easy_init();
        if (curl_) {
            setupDefaultOptions();
        }
    }
    
    ~Impl() {
        if (curl_) {
            curl_easy_cleanup(curl_);
        }
        curl_global_cleanup();
    }
    
    void setupDefaultOptions() {
        if (!curl_) return;
        
        curl_easy_setopt(curl_, CURLOPT_TIMEOUT, timeoutSeconds_);
        curl_easy_setopt(curl_, CURLOPT_USERAGENT, userAgent_.c_str());
        curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 2L);
    }
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
        size_t totalSize = size * nmemb;
        userp->append((char*)contents, totalSize);
        return totalSize;
    }
};

NetworkHandler::NetworkHandler() : impl_(new Impl()) {
}

NetworkHandler::~NetworkHandler() {
    delete impl_;
}

bool NetworkHandler::get(const std::string& url, std::string& response) {
    return get(url, {}, response);
}

bool NetworkHandler::get(const std::string& url, const std::map<std::string, std::string>& headers, std::string& response) {
    if (!impl_->curl_) {
        impl_->lastError_ = "CURL not initialized";
        return false;
    }
    
    response.clear();
    impl_->lastError_.clear();
    impl_->lastHttpCode_ = 0;
    
    // Set URL
    curl_easy_setopt(impl_->curl_, CURLOPT_URL, url.c_str());
    
    // Set write callback
    curl_easy_setopt(impl_->curl_, CURLOPT_WRITEFUNCTION, Impl::WriteCallback);
    curl_easy_setopt(impl_->curl_, CURLOPT_WRITEDATA, &response);
    
    // Set headers if provided
    struct curl_slist* headerList = nullptr;
    if (!headers.empty()) {
        for (const auto& header : headers) {
            std::string headerStr = header.first + ": " + header.second;
            headerList = curl_slist_append(headerList, headerStr.c_str());
        }
        curl_easy_setopt(impl_->curl_, CURLOPT_HTTPHEADER, headerList);
    }
    
    // Perform the request
    CURLcode res = curl_easy_perform(impl_->curl_);
    
    // Clean up headers
    if (headerList) {
        curl_slist_free_all(headerList);
    }
    
    if (res != CURLE_OK) {
        impl_->lastError_ = curl_easy_strerror(res);
        return false;
    }
    
    // Get HTTP response code
    curl_easy_getinfo(impl_->curl_, CURLINFO_RESPONSE_CODE, &impl_->lastHttpCode_);
    
    // Check for HTTP errors
    if (impl_->lastHttpCode_ >= 400) {
        impl_->lastError_ = "HTTP " + std::to_string(impl_->lastHttpCode_);
        return false;
    }
    
    return true;
}

void NetworkHandler::setTimeout(int timeoutSeconds) {
    impl_->timeoutSeconds_ = timeoutSeconds;
    if (impl_->curl_) {
        curl_easy_setopt(impl_->curl_, CURLOPT_TIMEOUT, timeoutSeconds);
    }
}

void NetworkHandler::setUserAgent(const std::string& userAgent) {
    impl_->userAgent_ = userAgent;
    if (impl_->curl_) {
        curl_easy_setopt(impl_->curl_, CURLOPT_USERAGENT, userAgent.c_str());
    }
}

std::string NetworkHandler::getLastError() const {
    return impl_->lastError_;
}

int NetworkHandler::getLastHttpCode() const {
    return impl_->lastHttpCode_;
}
