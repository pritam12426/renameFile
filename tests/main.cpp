#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include "functions.hpp"

namespace fs = std::filesystem;

TEST_CASE("FormatPathName", "[formatPathName]") {
    SECTION("Empty string") {
        fs::path p = "";
        REQUIRE(Fun::formatPathName(p) == false);
        REQUIRE(p == "");
    }

    SECTION("Simple lowercase") {
        fs::path p = "Test";
        REQUIRE(Fun::formatPathName(p) == true);
        REQUIRE(p == "test");
    }

    SECTION("Spaces to underscore") {
        fs::path p = "A B C";
        REQUIRE(Fun::formatPathName(p) == true);
        REQUIRE(p == "a_b_c");
    }

    SECTION("Multiple spaces to single underscore") {
        fs::path p = "A  B   C";
        REQUIRE(Fun::formatPathName(p) == true);
        REQUIRE(p == "a_b_c");
    }

    SECTION("Multiple underscores to single underscore") {
        fs::path p = "A__B___C";
        REQUIRE(Fun::formatPathName(p) == true);
        REQUIRE(p == "a_b_c");
    }

    SECTION("Leading and trailing underscores") {
        fs::path p = "_A_B_C_";
        REQUIRE(Fun::formatPathName(p) == true);
        REQUIRE(p == "a_b_c");
    }

    SECTION("Special characters") {
        fs::path p = "A!B@C#D$E%F^G&H*I(J)K";
        REQUIRE(Fun::formatPathName(p) == true);
        REQUIRE(p == "a!b@c#d$e%f^g&h*i(j)k");
    }

    SECTION("Special characters with underscores") {
        fs::path p = "A_!_B";
        REQUIRE(Fun::formatPathName(p) == true);
        REQUIRE(p == "a!b");
    }

    SECTION("Dots to underscores") {
        fs::path p = "file.name.txt";
        REQUIRE(Fun::formatPathName(p) == true);
        REQUIRE(p == "file_name_txt");
    }

    SECTION("Non-ASCII characters") {
        fs::path p = "file-with-äöü.txt";
        REQUIRE(Fun::formatPathName(p) == true);
        REQUIRE(p == "file-with-txt");
    }

    SECTION("Complex case") {
        fs::path p = "  A_B C-D.E F__G  ";
        REQUIRE(Fun::formatPathName(p) == true);
        REQUIRE(p == "a_b_c-d_e_f_g");
    }
}