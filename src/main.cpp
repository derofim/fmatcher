#include <iostream>
#include <string>
#include <regex>
#include <functional>

#include "argparse.hpp"

std::string to_forward_slashes(const std::string& str) {
    std::string str_copy(str);
    // Replace backward slashes with forwards slashes
    std::replace(str_copy.begin(), str_copy.end(), '\\', '/');
    return str_copy;
}

std::string regex_escape_special_characters(const std::string& real_pattern) {
    // matches any characters that need to be escaped in RegEx
    std::regex specialChars{R"([-[\]{}()*+?.,\^$|#\s])"};

    return std::regex_replace(real_pattern, specialChars, R"(\$&)");
}

void substr_replace(std::string& changing_str, const std::string& what,
                    const std::string& replacer) {
    std::string::size_type pos = 0u;
    while((pos = changing_str.find(what, pos)) != std::string::npos) {
        changing_str.replace(pos, what.length(), replacer);
        pos += replacer.length();
    }
}

std::string to_std_regex_pattern(const std::string& escaped_pattern) {
    std::string regex_pattern = escaped_pattern;

    // ? => match any 1 symbol except forward slash
    substr_replace(regex_pattern, R"RAW(\?)RAW", R"RAW([^/]+)RAW");

    // * => match any 0+ symbols
    substr_replace(regex_pattern, R"RAW(\*\*)RAW", R"RAW((.*))RAW");

    // ** => match any 0+ symbols except forward slash
    substr_replace(regex_pattern, R"RAW(\*)RAW", R"RAW([^/]*)RAW");

    return regex_pattern;
}

std::regex convert_regex(const std::string& real_pattern) {
    const std::string& fixed_pattern   = to_forward_slashes(real_pattern);
    const std::string& escaped_pattern = regex_escape_special_characters(fixed_pattern);
    std::cout << "escaped_pattern " << escaped_pattern << std::endl;
    const std::string& regex_pattern = to_std_regex_pattern(escaped_pattern);
    std::cout << "regex_pattern " << regex_pattern << std::endl;
    return std::regex(regex_pattern);
}

bool is_valid_filename(const std::string& filename) {
    // Prohibited filenames that contain one of (with space): *, ?
    return std::string::npos == filename.find_first_of(R"RAW(*, ?)RAW");
}

int main(int argc, char const* const* argv) {
    using namespace argparse;

    std::string filename, pattern;
    bool        showhelp = false;

    auto cli = Opt(filename, "filename")["-f"]["--filename"] |
               Opt(pattern, "pattern")["-p"]["--pattern"]
               //| Help(showhelp);
               | Opt(showhelp, "help")["-h"]["--help"]
            //.optional()
            //.flag()
            ;
    // TODO: refactor
    // TODO: TEST
    // TODO: isFlag
    // TODO: CustomCeck labmda https://mariusbancila.ro/blog/2018/04/03/parsing-command-line-arguments-in-cpp-with-clara/
    // TODO: required
    // TODO: stream
    // TODO: exact policy: no extra args
    // TODO: noopt policy: get args by pos
    // LIKE: ./fmatcher -f "pattern1\/28" rawarg0 -p "pattern*\/28" -h -flag rawarg1 rawarg2 -longstr Your long text --double
    // TODO: copy &
    // TODO: pass by const ref
    // TODO: header only
    cli.parse(Args(argc, argv));
    std::cout << "filename " << filename << std::endl;
    std::cout << "pattern " << pattern << std::endl;
    std::cout << "showhelp " << std::to_string(showhelp) << std::endl;
    //cli.debugprint();

    /*

    auto cli
        = Opt( filename, "filename" )
            ["-f"]["--filename"]
            ("What is file name?")
            .require()
        | Opt( pattern, "pattern" )
            ["-p"]["--pattern"]
            ("What is pattern?")
            .require()
         | Help(showhelp);*/

    /*auto cli
        = Opt( filename, "filename" )
            ["-f"]["--filename"]
            ("What is file name?")
            .required()
        | Opt( pattern, "pattern" )
            ["-p"]["--pattern"]
            ("What is pattern?")
            .required()
         | Help(showhelp);

    auto result = cli.parse( Args( argc, argv ) );
    if(!result) {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        exit(2);
    }

    if (result.value().type() != ParseResultType::Matched) {
        std::cerr << "Error in command line: not enough arguments" << std::endl;
        exit(2);
    }

    if (showhelp)
    {
        cli.writeToStream(std::cout);
        exit(2);
    }*/

    if(!is_valid_filename(filename)) {
        std::cerr << "invalid filename " << filename << std::endl;
        exit(1);
    }

    // Simple regular expression matching
    std::string fnames[] = {to_forward_slashes(filename)};
    std::cout << "filename " << to_forward_slashes(filename) << std::endl;

    for(const auto& fname : fnames) {
        std::cout << fname << ": " << std::regex_match(fname, convert_regex(pattern)) << '\n';
    }

    return 0;
}
