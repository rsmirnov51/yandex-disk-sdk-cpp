#include "catch.hpp"

#include <url/path.hpp>

using url::path;

SCENARIO("swap two difference paths", "[url][path][swap]") {

    GIVEN("difference paths p1 and p2") {

        path p1 = "/dir/file1";
        path p2 = "/dir/dir2/file2";
        path p3 = p1;
        path p4 = p2;

        CHECK(p1 != p2);

        WHEN("swap its") {

            swap(p1, p2);

            THEN("p1 must be swapped with p2") {

                REQUIRE(p1 == p4);
                REQUIRE(p2 == p3);
            }   
        }
    }
}

SCENARIO("swap two same paths", "[url][path][swap]") {

    GIVEN("same paths p1 and p2") {

        path p1 = "/dir/file1";
        path p2 = p1;

        CHECK(p1 == p2);

        WHEN("swap its") {

            swap(p1, p2);

            THEN("p1 must be equal p2") {

                REQUIRE(p1 == p2);
            }
        }
    }
}
