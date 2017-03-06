#include <stdio.h>
#include <curl/curl.h>
//using namespace std;
//#include <url/params.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
using std::string;

#include <list>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "url/path.hpp"

//#include "callbacks.hpp"
#include "quote.hpp"

 Client::Client(string token_) : token{token_} {}

auto upload(string url, url::path from, list<string> fields) -> json

  {
    CURL *curl;
  CURLcode res;
 url::params_t url_params;
		struct curl_slist *header_list = nullptr;
		std::string auth_header;
    
    url_params["path"] = quote(to.string(), curl);
    url_params["fields"] = boost::algorithm::join(fields, ",");
		std::string url = api_url + "/copy?" + url_params.string();
    auth_header = "Authorization: OAuth " + token;
		header_list = curl_slist_append(header_list, auth_header.c_str()); 
    stringstream response;
  // Инициализация
  curl_global_init(CURL_GLOBAL_ALL);
 
  curl = curl_easy_init();
   // Установка URL, который должен получить POST 
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    //Указываем данные POST
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=Nick&project=curl");
      curl_easy_setopt(curl, CURLOPT_READDATA, &response);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, write<stringstream>);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
    //Начало выполнения запроса
    auto res = curl_easy_perform(curl);
    //Проверка на ошибки 
   curl_slist_free_all(header_list);
		curl_easy_cleanup(curl);

		if (res != CURLE_OK) return json();

		auto answ = json::parse(response);
		return answ;
  

};

