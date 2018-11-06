/*
 * @file
 * @brief
 * Defines a very simple command line argument parser.
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

namespace argparse {

class Args;
class Opt;
class Parser;

class Args
{
private:
    std::string              m_exeName;
    std::vector<std::string> m_args;

public:
    Args(int argc, char const* const* argv)
            : m_exeName(argv[0])
            , m_args(argv + 1, argv + argc) {
    }

    Args(std::initializer_list<std::string> args)
            : m_exeName(*args.begin())
            , m_args(args.begin() + 1, args.end()) {
    }

    std::string getExeName() const {
        return m_exeName;
    }

    std::vector<std::string> getArgs() const {
        return m_args;
    }
};

struct OptValueBase
{
    virtual void setValue(const std::string& value) = 0;
};

template <typename T>
struct OptValue : OptValueBase
{
    T& m_value;

    OptValue(T& val)
            : m_value(val) {
    }

    template <typename Type>
    inline bool convertInto(const std::string& source, Type& target) {
        std::stringstream ss;
        ss << source;
        ss >> target;
        if(ss.fail())
            return false;
        else
            return true;
    }

    inline bool convertInto(const std::string& source, bool& target) {
        target = (source == "1") || (source == "y") || (source == "yes") || (source == "true") ||
                 (source == "on");
        return true;
    }

    void setValue(const std::string& value) override {
        //std::cout<<"OptValue " << value << std::endl;
        T    temp;
        auto result = convertInto(value, temp);
        if(!result) {
            std::cerr << "Error during type convertion" << std::endl;
            return;
        }
        m_value = temp;
    }
};

class Opt
{
    //private:
public:
    std::vector<std::string>      m_optNames;
    std::shared_ptr<OptValueBase> m_optValueRef;
    std::string                   m_Title;

public:
    template <typename T>
    Opt(T& value, const std::string& name)
            : m_optValueRef(std::make_shared<OptValue<T>>(OptValue<T>(value)))
            , m_Title(name) {
    }

    Opt& operator[](std::string const& optName) {
        m_optNames.push_back(optName);
        return *this;
    }

    bool isMatch(const std::string& other) const {
        for(const auto& name : m_optNames) {
            if(name == other)
                return true;
        }
        return false;
    }

    void parse(Args args);

    Parser operator|(Opt const& other) const;
};

class Parser
{
private:
    std::vector<Opt> m_opts;

public:
    Parser(const Opt& opt) {
        m_opts.push_back(opt);
    }

    Parser& operator|(Opt const& other) {
        m_opts.push_back(other);
        return *this;
    }

    void debugprint() {
        std::cout << std::endl;
        for(const Opt& item : m_opts) {
            std::cout << std::endl;
            std::cout << item.m_Title << std::endl;
            for(const std::string& alias : item.m_optNames) {
                std::cout << alias << std::endl;
            }
        }
    }

    void parse(Args args) {
        std::cout << std::endl;
        //for (const std::string& item : args.getArgs()) {
        const std::vector<std::string>& argsVec = args.getArgs();
        for(size_t i = 0; i < argsVec.size(); i++) {
            const std::string& item = argsVec.at(i);
            //std::cout << item << std::endl;
            for(Opt& option : m_opts) {
                if(option.isMatch(item)) {
                    std::string argVal = "";
                    if(i < argsVec.size() - 1) { //TODO: if not flag
                        argVal = argsVec[i + 1];
                    }
                    option.m_optValueRef->setValue(argVal);
                    std::cout << "=> " << option.m_Title << " : " << item << " : " << argVal
                              << std::endl;
                }
            }
        }
    }
};

Parser Opt::operator|(Opt const& other) const {
    return Parser(*this) | other;
}

void Opt::parse(Args args) {
    return Parser(*this).parse(args);
}

} // namespace argparse