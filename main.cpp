#include <iostream>
#include <string>
#include <regex>
#include <functional>

#include "clara.hpp"

using namespace clara;

std::string to_forward_slashes(const std::string& str)
{
    std::string str_copy(str);
    // Replace backward slashes with forwards slashes
    std::replace(str_copy.begin(), str_copy.end(), '\\', '/');
    return str_copy;
}

std::string regex_escape_special_characters(const std::string& real_pattern)
{
    // matches any characters that need to be escaped in RegEx
    std::regex specialChars { R"([-[\]{}()*+?.,\^$|#\s])" };

    return std::regex_replace( real_pattern, specialChars, R"(\$&)" );
}

void substr_replace(std::string& changing_str, const std::string& what,
    const std::string& replacer)
{
    std::string::size_type pos = 0u;
    while((pos = changing_str.find(what, pos)) != std::string::npos) {
        changing_str.replace(pos, what.length(), replacer);
        pos += replacer.length();
    }
}

std::string to_std_regex_pattern(const std::string& escaped_pattern)
{
    std::string regex_pattern = escaped_pattern;

    // ? => match any 1 symbol except forward slash
    substr_replace(regex_pattern, R"RAW(\?)RAW", R"RAW([^/]+)RAW");

    // * => match any 0+ symbols
    substr_replace(regex_pattern, R"RAW(\*\*)RAW", R"RAW((.*))RAW");

    // ** => match any 0+ symbols except forward slash
    substr_replace(regex_pattern, R"RAW(\*)RAW", R"RAW([^/]*)RAW");

    return regex_pattern;
}

std::regex convert_regex(const std::string& real_pattern)
{
    const std::string& fixed_pattern = to_forward_slashes(real_pattern);
    const std::string& escaped_pattern = regex_escape_special_characters(fixed_pattern);
    std::cout << "escaped_pattern " << escaped_pattern << std::endl;
    const std::string& regex_pattern = to_std_regex_pattern(escaped_pattern);
    std::cout << "regex_pattern " << regex_pattern << std::endl;
    return std::regex(regex_pattern);
}

int main(int argc, char const * const * argv)
{
    std::string filename, pattern;

    auto cli
        = Opt( filename, "filename" )
            ["-f"]["--filename"]
            ("What is file name?")
        | Opt( pattern, "pattern" )
            ["-p"]["--pattern"]
            ("What is pattern?");

    auto result = cli.parse( Args( argc, argv ) );
    if(!result) {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        exit(2);
    }

/*Prohibited filenames that contain one of (with space):
*, ?*/

    // Simple regular expression matching
    std::string fnames[] = {filename};
 
    for (const auto &fname : fnames) {
        std::cout << fname << ": " << std::regex_match(fname, convert_regex(pattern)) << '\n';
    }
 

    return 0;
}
