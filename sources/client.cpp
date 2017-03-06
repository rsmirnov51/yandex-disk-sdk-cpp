#include <curl/curl.h>
#include <stdio.h>
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
    		auto Client::upload(url::path to,string api_url, std::list<string> fields) -> json

  {
    		 CURL *curl;
 		
 	
		 struct curl_slist *header_list = nullptr;
		
   		 url::params_t url_params;
   		 url_params["path"] = quote(to.string(), curl);
  		 url_params["fields"] = boost::algorithm::join(fields, ",");
		 std::string url = api_url + "/copy?" + url_params.string();
		 curl_slist * header_list = nullptr;
  		 std::string auth_header = "Authorization: OAuth " + token;
		 header_list = curl_slist_append(header_list, auth_header.c_str()); 
                 stringstream res;
 
 		 curl = curl_easy_init();
   // Установка URL, который должен получить POST 
  		  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    //Указываем данные POST
   
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

		auto answ = json::parse(res);
		return answ;
  

}

};

