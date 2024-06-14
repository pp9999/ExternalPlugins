#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <chrono>
#include <unordered_map>
#include "curl.h"

class HttpClient {
public:

    enum class RequestMethod {
        GET,
        POST
    };

    class HttpResponse {
    public:
        HttpResponse() = default;
        HttpResponse(std::string_view body, std::string_view headers);

        std::string& GetBody() {
            return m_body;
        }

        const std::string& GetBody() const {
            return m_body;
        }

        std::chrono::steady_clock::time_point GetTimestamp() const;
        std::string GetHeaderValue(const std::string& headerName) const;

    private:
        void ParseHeaders(std::string_view headers);

        std::string m_body;
        std::chrono::steady_clock::time_point m_timestamp;
        std::vector<std::pair<std::string, std::string>> m_headers;
    };

    static HttpResponse Request(std::string_view url, RequestMethod method, const std::vector<std::string>& fields = {}, std::chrono::minutes cacheLimit = std::chrono::minutes(30));

private:
    static size_t WriteFunction(void* contents, size_t size, size_t nmemb, HttpResponse* response);
    static std::unordered_map<std::string, HttpResponse> responseCache;
};