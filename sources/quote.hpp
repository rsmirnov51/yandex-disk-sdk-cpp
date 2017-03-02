#include <curl/curl.h>
#include <url/path.hpp>

std::vector<std::string> split(const std::string& text, const std::string& delims)
{
    std::vector<std::string> tokens;
    std::size_t start = text.find_first_not_of(delims), end = 0;

    while ((end = text.find_first_of(delims, start)) != std::string::npos)
    {
        tokens.push_back(text.substr(start, end - start));
        start = text.find_first_not_of(delims, end);
    }
    if (start != std::string::npos)
        tokens.push_back(text.substr(start));

    return tokens;
}

auto quote(const url::path& path, CURL * curl) -> std::string {

    if (is_root(path)) return path.string();

    auto names = split(path.string(), url::path::separator);
    std::string quote_path;
    std::for_each(names.begin(), names.end(), [&quote_path, curl](std::string& name) {
        auto escape_name = curl_easy_escape(curl, name.c_str(), name.size());
        quote_path.append(url::path::separator);
        quote_path.append(escape_name);
    });

    if (is_directory(path)) quote_path.append(url::path::separator);
    return quote_path;
}
