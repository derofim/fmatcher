#include <iostream>
#include <stdexcept>
#include "../src/argparse.hpp"
#include "tinytest.hpp"

int main(int argc, char const* const* argv) {
    std::cout << "Test arg!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

    TEST_CASE("test_case1", {
        std::cerr << "test_case1!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        EXPECT(1 == 1);
        EXPECT_FAIL(2 == 1);
    });

    TEST_CASE("test_case2", {
        EXPECT_THROWS({ throw std::runtime_error("Error 1"); }, std::runtime_error);
        // EXPECT_THROWS({ throw std::runtime_error("Error 1"); }, std::underflow_error);
        std::cerr << "test_case1!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        EXPECT(1 == 1);
        EXPECT_FAIL(2 == 1);
    });

    return tinytest::run_tests();
}