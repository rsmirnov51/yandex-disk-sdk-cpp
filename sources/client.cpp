#include <curl/curl.h>

#include <url/params.hpp>
#include <yadisk/client.hpp>
#include <boost/algorithm/string/join.hpp>

#include <sstream>
using std::stringstream;

#include "callbacks.hpp"
#include "quote.hpp"

namespace yadisk
{
    static const std::string api_url = "https://cloud-api.yandex.net/v1/disk/resources";

    Client::Client(string token_) : token{token_} {}

	auto Client::copy(url::path from, url::path to, bool overwrite, std::list<string> fields = nullptr) -> json {
		CURL * curl;
		url::params_t url_params;
		struct curl_slist *header_list = nullptr;
		std::string auth_header;
		/*
		 *	from=<путь к копируемому ресурсу>
		 *	& path=<путь к скопированному ресурсу>
		 *	[& overwrite=<признак перезаписи>]
		 *	[& fields=<нужные ключи ответа>]
		*/
		
		curl = curl_easy_init();
		if (!curl) return "";
		
		url_params["from"] = quote(from.string(), curl);
		url_params["path"] = quote(to.string(), curl);
		url_params["overwrite"] = overwrite;
		url_params["fields"] = boost::algorithm::join(fields, ",");
		std::string url = api_url + "/copy?" + url_params.string();

		auth_header = "Authorization: OAuth " + token;
		header_list = curl_slist_append(header_list, auth_header.c_str());

		stringstream response;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_READDATA, &response);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, write<stringstream>);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

		auto response_code = curl_easy_perform(curl);

		curl_slist_free_all(header_list);
		curl_easy_cleanup(curl);

		if (response_code != CURLE_OK) return json();

		auto answ_data = json::parse(response);
		return answ_data;
	}
    
    auto Client::patch(url::path resource, json meta, std::list<string> fields) -> json {

        // init http request
        CURL * curl = curl_easy_init();

        // fill http url
        url::params_t url_params;
        url_params["fields"] = boost::algorithm::join(fields, ",");
        url_params["path"] = quote(resource.string(), curl);
        std::string url = api_url + "?" + url_params.string();

        // fill http header
        curl_slist * http_header = nullptr;
        std::string authorization_header = "Authorization: OAuth " + token;
        http_header = curl_slist_append(http_header, "Content-Type: application/json");
        http_header = curl_slist_append(http_header, authorization_header.c_str());

        // fill http body
        auto request_body = meta.dump();

        // build http request
        stringstream response_body;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write<stringstream>);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request_body.size());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_header);

        // perform http request
        auto response_code = curl_easy_perform(curl);

        // clean resources
        curl_slist_free_all(http_header);
        curl_easy_cleanup(curl);

        // check response code
        if ( response_code != CURLE_OK ) return json();

        // handle body of http response
        auto info = json::parse(response_body);
        return info;
    }
}
