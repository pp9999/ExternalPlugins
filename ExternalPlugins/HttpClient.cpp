#pragma once
#include "HttpClient.h"

std::unordered_map<std::string, HttpClient::HttpResponse> HttpClient::responseCache;

HttpClient::HttpResponse::HttpResponse(std::string_view body, std::string_view headers) {
    ParseHeaders(headers);
    m_body = body;
    m_timestamp = std::chrono::steady_clock::now();
}

std::chrono::steady_clock::time_point HttpClient::HttpResponse::GetTimestamp() const {
    return m_timestamp;
}

void HttpClient::HttpResponse::ParseHeaders(std::string_view headers) {
    std::istringstream headerStream(headers.data());
    std::string line;

    while (std::getline(headerStream, line) && !line.empty()) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string headerName = line.substr(0, colonPos);
            std::string headerValue = line.substr(colonPos + 1);
            m_headers.emplace_back(std::move(headerName), std::move(headerValue));
        }
    }
}

std::string HttpClient::HttpResponse::GetHeaderValue(const std::string& headerName) const {
    auto it = std::find_if(m_headers.begin(), m_headers.end(),
        [&headerName](const auto& pair) { return pair.first == headerName; });

    return (it != m_headers.end()) ? it->second : std::string();
}

size_t HttpClient::WriteFunction(void* contents, size_t size, size_t nmemb, HttpResponse* response) {
    size_t totalSize = size * nmemb;
    const char* data = static_cast<const char*>(contents);
    response->GetBody().append(data, totalSize);
    return totalSize;
}

HttpClient::HttpResponse HttpClient::Request(std::string_view url, RequestMethod method, const std::vector<std::string>& fields, std::chrono::minutes cacheLimit) {
    auto cacheKey = std::string(url);
    if (method == RequestMethod::GET) {        
        auto cacheIt = responseCache.find(cacheKey);
        if (cacheIt != responseCache.end()) {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::minutes>(currentTime - cacheIt->second.GetTimestamp()).count();
            if (elapsedTime < cacheLimit.count()) {
                return cacheIt->second;
            }
        }
    }
    HttpResponse response("", "");

    curl_global_init(CURL_GLOBAL_DEFAULT);
    auto curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");

        if (method == RequestMethod::POST) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.empty() ? nullptr : fields[0].c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fields.empty() ? 0 : fields[0].size());
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (res != CURLE_OK) {
            //console_text  << "Failed to perform curl request: " << curl_easy_strerror(res) << std::endl;
            //DebugImGui.Debugtext_addstream();
        }

        if (method == RequestMethod::GET) {
            responseCache[cacheKey] = response;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    else {
        //console_text  << "Failed to initialize curl" << std::endl;
        //DebugImGui.Debugtext_addstream();
    }

    return response;
}
