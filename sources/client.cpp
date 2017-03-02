#include <stdio.h>
#include <curl/curl.h>
//using namespace std;

class Client {
public:

  void UploadN()
  {
    CURL *curl;
  CURLcode res;
 
  // Инициализация
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    // Установка URL, который должен получить POST 
    curl_easy_setopt(curl, CURLOPT_URL, "http://postit.example.com/moo.cgi");
    //Указываем данные POST
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=Nick&project=curl");
 
    //Начало выполнения запроса
    res = curl_easy_perform(curl);
    //Проверка на ошибки 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    //Очистка 
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
  }

};

