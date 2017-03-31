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
	static const std::string api_url = "https://cloud-api.yandex.net/v1/disk";

	Client::Client(string token_) : token{token_} {}

	auto Client::ping() -> bool {

		CURL * curl = curl_easy_init();
		if (curl == nullptr) return false;

		std::string url = api_url;

		struct curl_slist *header_list = nullptr;
		std::string auth_header = "Authorization: OAuth " + token;
		header_list = curl_slist_append(header_list, auth_header.c_str());

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

		auto response_code = curl_easy_perform(curl);
		
		long http_response_code = 0;
		if (response_code == CURLE_OK) {
			response_code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);			
		}	
		
		curl_slist_free_all(header_list);
		curl_easy_cleanup(curl);

		if (response_code != CURLE_OK) return false;

		return http_response_code == 200;
	}

	auto Client::copy(url::path from, url::path to, bool overwrite, std::list<std::string> fields) -> json {

		CURL * curl = curl_easy_init();
		if (!curl) return json();

		url::params_t url_params;
		url_params["from"] = quote(from.string(), curl);
		url_params["path"] = quote(to.string(), curl);
		url_params["overwrite"] = overwrite;
		url_params["fields"] = boost::algorithm::join(fields, ",");
		std::string url = api_url + "/resources/copy" + "?" + url_params.string();

		struct curl_slist *header_list = nullptr;
		std::string auth_header = "Authorization: OAuth " + token;
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

		auto response_data = json::parse(response);
		return response_data;
	}
    
	auto Client::patch(url::path resource, json meta, std::list<string> fields) -> json {

		// init http request
		CURL * curl = curl_easy_init();
		if (curl == nullptr) return json();

		// fill http url
		url::params_t url_params;
		url_params["fields"] = boost::algorithm::join(fields, ",");
		url_params["path"] = quote(resource.string(), curl);
		std::string url = api_url + "/resources" + "?" + url_params.string();

		// fill http headers
		curl_slist * header_list = nullptr;
		std::string auth_header = "Authorization: OAuth " + token;
		header_list = curl_slist_append(header_list, "Content-Type: application/json");
		header_list = curl_slist_append(header_list, auth_header.c_str());

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
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

		// perform http request
		auto response_code = curl_easy_perform(curl);

		// clean resources
		curl_slist_free_all(header_list);
		curl_easy_cleanup(curl);

		// check response code
		if ( response_code != CURLE_OK ) return json();

		// handle body of http response
		auto info = json::parse(response_body);
		return info;
	}
	auto Client::publish(url::path resource) -> json
	{
		CURL *curl;
		FILE *fd;
		url::params_t url_params;
		struct curl_slist *header_list = nullptr;
		std::string auth_header;
		
		fs::path p (fs::current_path());
		url_params["path"] = quote(to.string(), curl);
		std::string url = api_url + "/publish" + url_params.string();
		
		struct curl_slist *head_list = nullptr;
		auth_header = "Authorization: OAuth " + token;
		head_list = curl_slist_append(head_list, auth_header.c_str());
		stringstream res;
		
		curl = curl_easy_init();
		
		/* get the file size of the local file */ 
  		stat(file, &file_info);
		hd_src = fopen(file, "rb");
		curl_global_init(CURL_GLOBAL_ALL);
		/* we want to use our own read function */ 
  		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);  
		
		/* HTTP PUT please */ 
		curl_easy_setopt(curl, CURLOPT_PUT, 1L);

		/* specify target URL, and note that this URL should include a file
		   name, not only a directory */ 
		curl_easy_setopt(curl, CURLOPT_URL, url);
		
		auto_res_code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(header_list);
		if (res_code != CURLE_OK) return json();
		
		return 0;
	}
}

class curl_environment {
public:
    curl_environment() {
        curl_global_init(CURL_GLOBAL_ALL);
    }
    ~curl_environment() {
        curl_global_cleanup();
    }
};

static const curl_environment env;

