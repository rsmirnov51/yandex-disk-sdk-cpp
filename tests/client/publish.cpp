#include <catch.hpp>
#include <yadisk/client.hpp>
using ydclient = yadisk::Client;

#include <string>
#include <list>

#include <url/path.hpp>
using url::path;

TEST_CASE("publish valid file with valid token", "[client][publish][file]") {

	std::string token = "AQAAAAATPnx3AAQXOJS1w4zmPUdrsJNR1FATxEM";
	path resource{ "/file.dat" };
	ydclient client{ token };
	auto info = client.publish(resource);
	REQUIRE(not info.empty());
	auto msg = info["href"].get<std::string>();
	REQUIRE(msg == "https://cloud-api.yandex.net/v1/disk/resources?path=disk%3A%2Ffile.dat");
}

TEST_CASE("publish with invalid token", "[client][publish][file]") {

	std::string token = "invalid-token";
	path resource{ "/file.dat" };
	ydclient client{ token };
	auto info = client.publish(resource);
	REQUIRE(not info.empty());
	auto msg = info["error"].get<std::string>();
	REQUIRE(msg == "UnauthorizedError");
}

TEST_CASE("publish invalid file with valid token", "[client][publish][file]") {

	std::string token = "AQAAAAATPnx3AAQXOJS1w4zmPUdrsJNR1FATxEM";
	path resource{ "/valid-file.dat" };
	ydclient client{ token };
	auto info = client.publish(resource);
	REQUIRE(not info.empty());
	auto msg = info["error"].get<std::string>();
	REQUIRE(msg == "DiskNotFoundError");
}

TEST_CASE("publish valid dir with valid token", "[client][publish][dir]") {

	std::string token = "AQAAAAATPnx3AAQXOJS1w4zmPUdrsJNR1FATxEM";
	path resource{ "/valid-dir/" };
	ydclient client{ token };
	auto info = client.publish(resource);
	REQUIRE(not info.empty());
	auto msg = info["href"].get<std::string>();
	REQUIRE(msg == "https://cloud-api.yandex.net/v1/disk/resources?path=valid-dir");
}
