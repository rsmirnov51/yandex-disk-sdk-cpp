#include "json.hpp"
using json = nlohmann::json;

#include <string>
using std::string;

#include <list>
using list = std::list<std::string>;

#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;

#include "url/path.hpp"

namespace YandexDisk
{
    class Client 
    {
    public:
    
        Client(string token);
        
        auto info() -> json;
  
        auto info(url::path resource, json options = nullptr) -> json;
  
        auto list(json fields = nullptr) -> json;
        
        auto upload(url::path to, fs::path from, bool overwrite, list fields = nullptr) -> json;
  
        auto upload(url::path to, url::path from, list fields = nullptr) -> json;
  
        auto download(url::path from, url::path to, list fields = nullptr) -> json;
  
        auto copy(url::path from, url::path to, bool overwrite, list fields = nullptr) -> json;
  
        auto move(url::path from, url::path to, bool overwrite, list fields = nullptr) -> json;
        
        auto mkdir(url::path dir, list fields = nullptr) -> json;
        
        auto remove(url::path resource, bool permanently, list fields = nullptr) -> json;
  
        auto publish(url::path resource) -> json;
  
        auto unpublish(url::path resource) -> json;
        
        string token;
    };

    auto info(string public_key, url::path resource = nullptr, json options = nullptr) -> json;
  
    auto download(string public_key, fs::path to, url::path file = nullptr)-> json;
  
    auto save(string public_key, fs::path to, url::path file = nullptr)-> json;
}
