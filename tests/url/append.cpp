#include "catch.hpp"

#include <url/path.hpp>

using url::path;

TEST_CASE("append dir with dir", "[url][path][op/]") {

    REQUIRE(path("/dir1/") / path("/dir2/") == path("/dir1/dir2/"));
    REQUIRE(path("/dir1/") / path("dir2/")  == path("/dir1/dir2/"));
    REQUIRE(path("dir1/")  / path("/dir2/") == path("dir1/dir2/"));
    REQUIRE(path("dir1/")  / path("dir2/")  == path("dir1/dir2/"));
}


TEST_CASE("append dir with file", "[url][path][op/]") {

    REQUIRE(path("/dir/")  / path("/file")  == path("/dir/file"));
    REQUIRE(path("/dir/")  / path("file")   == path("/dir/file"));
    REQUIRE(path("dir/")   / path("/file")  == path("dir/file"));
    REQUIRE(path("dir/")   / path("file")   == path("dir/file"));
}


TEST_CASE("append file with dir", "[url][path][op/]") {

    REQUIRE(path("/file")  / path("/dir/")   == path("/file/dir/"));
    REQUIRE(path("/file")  / path("dir/")    == path("/file/dir/"));
    REQUIRE(path("file")   / path("/dir/")   == path("file/dir/"));
    REQUIRE(path("file")   / path("dir/")    == path("file/dir/"));
}


TEST_CASE("append file with file", "[url][path][op/]") {

    REQUIRE(path("/file1") / path("/file2")  == path("/file1/file2"));
    REQUIRE(path("/file1") / path("file2")   == path("/file1/file2"));
    REQUIRE(path("file1")  / path("/file2")  == path("file1/file2"));
    REQUIRE(path("file1")  / path("file2")   == path("file1/file2"));
}
