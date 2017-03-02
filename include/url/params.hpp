#ifndef URL_PARAMS_HPP
#define URL_PARAMS_HPP

#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace url
{
    using dict_t = std::map<std::string, std::string>;

    class params_t
    {
    public:

        params_t() = default;

        params_t(dict_t params) : m_dict{params} {}

        params_t(const params_t&) = default;

        params_t(params_t&&) = default;

        auto operator=(const params_t&) -> params_t& = default;

        auto operator=(params_t&&) -> params_t& = default;

        auto operator[](const std::string& key) -> std::string& {
            return m_dict[key];
        }

        friend auto operator<<(std::ostream& out, const url::params_t& params) -> std::ostream&;

        auto string() const -> std::string {
            std::stringstream ss;
            ss << *this;
            return ss.str();
        }

    private:

        dict_t m_dict;
    };

    auto operator<<(std::ostream& out, const url::params_t& params) -> std::ostream& {

        std::vector<std::string> pairs;
        for_each(params.m_dict.begin(), params.m_dict.end(), [&pairs](url::dict_t::value_type item) {
            pairs.push_back(item.first + "=" + item.second);
        });

        std::ostream_iterator<std::string> out_it(out, "&");
        std::copy(pairs.begin(), pairs.end(), out_it);

        return out;
    }
}



#endif
