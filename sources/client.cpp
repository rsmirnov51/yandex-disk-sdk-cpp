#include <curl/curl.h>

#include <yadisk/client.hpp>
#include <url/params.hpp>
#include <boost/algorithm/string/join.hpp>

#include <sstream>
using std::stringstream;

#include "callbacks.hpp"
#include "quote.hpp"

namespace YandexDisk
{
    static const std::string api_url = "https://cloud-api.yandex.net/v1/disk/resources";

    Client::Client(string token_) : token{token_} {}

    auto Client::patch(url::path resource, json meta, std::list<string> fields) -> json {

        CURL * curl = curl_easy_init();
        url::params_t params;
        params["fields"] = boost::algorithm::join(fields, ",");
        params["path"] = quote(resource.string(), curl);
        stringstream body, response;
        std::string url = api_url + "?" + params.string();
        auto content = meta.dump();
        body << content;
        std::string text = "Authorization: OAuth " + token;

        curl_slist * header = nullptr;
        header = curl_slist_append(header, "Content-Type: application/json");
        header = curl_slist_append(header, text.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write<stringstream>);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, content.size());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
#ifdef DEBUG
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
#endif

        auto result = curl_easy_perform(curl);
        curl_slist_free_all(header);

        return result == CURLE_OK ? json::parse(response) : json();
    }
}
