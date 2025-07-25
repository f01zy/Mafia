#include "Http.h"
#include "../Config/Config.h"
#include <curl/curl.h>
#include <curl/easy.h>

size_t Http::WriteCallback(void *contents, size_t size, size_t nmemb,
                           void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string Http::request(std::string endpoint, std::string data) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  Config &config = Config::getInstance();

  if (curl) {
    std::string url = config.API_URL + "/api" + endpoint;

    if (data.size() > 0) {
      struct curl_slist *headers = nullptr;
      headers = curl_slist_append(
          headers, "Content-Type: application/x-www-form-urlencoded ");

      curl_easy_setopt(curl, CURLOPT_POST, 1L);
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return readBuffer;
  }

  return nullptr;
}
