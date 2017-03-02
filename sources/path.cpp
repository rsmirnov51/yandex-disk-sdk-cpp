#include <url/path.hpp>

#include <boost/algorithm/string/predicate.hpp>

using boost::algorithm::ends_with;
using boost::algorithm::starts_with;

namespace url
{

    std::string path::separator = "/";

    path::path(std::string str_) : str(str_) {}

    path::path(const char * str_) : str(str_) {}

    void path::swap(path& rhs) {
        std::swap(str, rhs.str);
    }

    path& path::operator+=(const path& rhs) {
        str += rhs.str;
        return *this;
    }

    path& path::operator/=(const path& rhs) {
        auto new_path = *this / rhs;
        this->swap(new_path);
        return *this;
    }

    path operator/(const path& lhs, const path& rhs) {
        if (starts_with(rhs.str, path::separator)){
            if (ends_with(lhs.str, path::separator)) {
               return lhs.str + rhs.str.substr(1);
            } else {
                return lhs.str + rhs.str;
            }
        }
        else {
            if (ends_with(lhs.str, path::separator)) {
                return lhs.str + rhs.str;
            }
            else {
                return lhs.str + path::separator + rhs.str;
            }
        }
    }

    path operator+(const path& lhs, const path& rhs) {
        return lhs.str + rhs.str;
    }

    bool path::operator==(const path& rhs) {
        return str == rhs.str;
    }

    bool path::operator!=(const path& rhs) {
        return str != rhs.str;
    }

    auto operator<<(std::ostream& out, const path& p) -> std::ostream& {
        out << p.str;
        return out;
    }

    auto path::string() const -> std::string {
        return str;
    }
}

auto swap(url::path& p1, url::path& p2) -> void {
    p1.swap(p2);
}

auto is_root(const url::path& p) -> bool {
    return p.string() == url::path::separator;
}

auto is_directory(const url::path& p) -> bool {
    return boost::algorithm::ends_with(p.string(), url::path::separator);
}
