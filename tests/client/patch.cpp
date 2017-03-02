#include <catch.hpp>
#include <yadisk/client.hpp>
using ydclient = yadisk::Client;

#include <string>
#include <list>

#include <url/path.hpp>
using url::path;

TEST_CASE("adding meta info to file", "[client][set][meta][file]") {

    std::string token = "AQAAAAATPnx3AAQXOJS1w4zmPUdrsJNR1FATxEM";
    path resource{ "/file.dat" };
    ydclient client{ token };
    json meta = "{\"custom_properties\":{\"foo\":\"1\",\"bar\":\"2\"}}"_json;
    std::list<std::string> fields {"custom_properties.foo", "custom_properties.bar"};
    auto info = client.patch(resource, meta, fields);
    REQUIRE(not info.empty());
    auto foo = info["custom_properties"]["foo"].get<std::string>();
    auto bar = info["custom_properties"]["bar"].get<std::string>();
    REQUIRE(foo == "1");
    REQUIRE(bar == "2");
}
