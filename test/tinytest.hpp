/*
 * @file
 * @brief
 * Defines a very simple unit testing framework.
 *
 * @copyright
 * Copyright (c) 2018 Denis Trofimov
 *
 * @copyright
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * @copyright
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * @copyright
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include <ostream>
#include <sstream>
#include <vector>
#include <memory>
#include <cstddef>
#include <iostream>
#include <vector>

#if defined(__unix__) || defined(__unix) || defined(__linux__)
#define OS_LINUX
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define OS_WIN
#elif defined(__APPLE__) || defined(__MACH__)
#define OS_MAC
#else
#error Unknown Platform
#endif

#if defined(OS_LINUX) || defined(OS_MAC)
#include <unistd.h>
#elif defined(OS_WIN)
#include <windows.h>
#include <io.h>
#endif

namespace termcolor {

enum class color
{
    red             = 31,
    green           = 32,
    blue            = 34,
    reset           = 39,
    red_backgroud   = 41,
    green_backgroud = 42,
    blue_backgroud  = 44,
    reset_backgroud = 49
};

#if defined(OS_LINUX) || defined(OS_MAC)
template <typename T>
inline void set(std::ostream& os, T const value) {
    os << "\033[" << static_cast<int>(value) << "m";
}
#elif defined(OS_WIN)
enum class AttrColor : BYTE
{
    black   = 0,
    red     = 4,
    green   = 2,
    yellow  = 6,
    blue    = 1,
    magenta = 5,
    cyan    = 3,
    gray    = 7
};

inline BYTE ansi2attr(BYTE rgb) noexcept {
    static const AttrColor rev[8] = {AttrColor::black,  AttrColor::red,  AttrColor::green,
                                     AttrColor::yellow, AttrColor::blue, AttrColor::magenta,
                                     AttrColor::cyan,   AttrColor::gray};
    return static_cast<BYTE>(rev[rgb]);
}

template <typename T>
inline void set(std::ostream& os, T const value) {
    const HANDLE h = getConsoleHandle(os.rdbuf());
    if(h != INVALID_HANDLE_VALUE) {
        // Out all buffered text to console with previous settings:
        os.flush();
        SetConsoleTextAttribute(h, ansi2attr(static_cast<BYTE>(value) - 40));
    }
}
#endif

template <typename T>
using enableStd = typename std::enable_if<std::is_same<T, color>::value, std::ostream&>::type;

template <typename T>
inline enableStd<T> operator<<(std::ostream& os, const T value) {
    set(os, value);
    return os;
}

} // namespace termcolor

namespace tinytest {

size_t nPass = 0;
size_t nFail = 0;

struct TestCase
{
    const char* name;
    int (*function)();
    bool result;
};

std::vector<TestCase> testCases;

void on_succeed() {
    ++nPass;
}

void on_failed() {
    ++nFail;
}

void on_fail(const char* text, const char* fileName, long lineNumber) {
    using ::termcolor::color;

    std::cout << color::red;
    std::cout << "FAILURE: " << text << " in file " << fileName << " on line " << lineNumber
              << std::endl;
    std::cout << color::reset;
    on_failed();
}

int expect_bool(const bool expectation, const char* condText, bool cond, const char* fileName,
                long lineNumber) {
    if(cond != expectation) {
        on_fail(condText, fileName, lineNumber);
        return EXIT_FAILURE;
    } else {
        on_succeed();
    }
    return EXIT_SUCCESS;
}

int getTestsStatus() {
    if(nFail) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int run_tests() {
    using ::termcolor::color;

    for(TestCase& testCase : testCases) {
        std::cout << "==================================" << std::endl;
        std::cout << "Running test case " << testCase.name << std::endl;
        testCase.result = EXIT_SUCCESS == (*testCase.function)();
        std::cout << (testCase.result ? color::green : color::red);
        std::cout << (testCase.result ? "Passed" : "Failed") << " test case: " << testCase.name
                  << std::endl;
        std::cout << color::reset;
    }
    std::cout << "==================================" << std::endl;
    std::cout << "Results per test case: " << std::endl;
    for(const TestCase& testCase : testCases) {
        std::cout << (testCase.result ? color::green : color::red);
        std::cout << testCase.name << ": " << (testCase.result ? "Passed" : "Failed") << std::endl;
        std::cout << color::reset;
    }
    std::cout << "==================================" << std::endl;
    std::cout << "Result (Checked " << nPass + nFail << " tests):" << std::endl;
    std::cout << "Number of Passed tests = " << nPass << std::endl;
    std::cout << ((nFail == 0) ? color::green : color::red);
    std::cout << "Number of Failed tests = " << nFail << std::endl;
    std::cout << color::reset;

    return getTestsStatus();
}

} // namespace tinytest

#define STR_HELPER(x) #x

#define TO_STR(x) STR_HELPER(x)

#define EXPECT(cond)                                                                               \
    return tinytest::expect_bool(true, "EXPECT(" TO_STR(cond) ")", cond, __FILE__, __LINE__)

#define EXPECT_FAIL(cond)                                                                          \
    return tinytest::expect_bool(false, "EXPECT_FAIL(" TO_STR(cond) ")", cond, __FILE__, __LINE__)

#define EXPECT_THROWS(expr, T)                                                                     \
    try {                                                                                          \
        expr;                                                                                      \
        std::cout << "THROW ";                                                                     \
        tinytest::on_fail(TO_STR(expr), __FILE__, __LINE__);                                       \
    } catch(T) { tinytest::on_succeed(); } catch(...) {                                            \
        std::cout << "THROW ";                                                                     \
        tinytest::on_fail(TO_STR(expr), __FILE__, __LINE__);                                       \
    }

#define EXPECT_NO_THROW(expr)                                                                      \
    try {                                                                                          \
        expr;                                                                                      \
        tinytest::on_succeed();                                                                    \
    } catch(...) {                                                                                 \
        std::cout << "NOTHROW ";                                                                   \
        tinytest::on_fail(TO_STR(expr), __FILE__, __LINE__);                                       \
    }

#define TEST_CASE(name, code)                                                                      \
    tinytest::testCases.push_back({name,                                                           \
                                   []() {                                                          \
                                       code;                                                       \
                                       return EXIT_SUCCESS;                                        \
                                   },                                                              \
                                   false});