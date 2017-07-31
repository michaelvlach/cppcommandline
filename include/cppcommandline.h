#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <regex>
#include <memory>

namespace cppcommandline
{

class Option
{
public:
    enum class Type
    {
        Undefined,
        Boolean,
        Integer,
        Double,
        LongLong,
        String
    };

    Option() {}
    explicit Option(std::string longName) {}

    bool isPositional() const { return false; }
    std::string longName() const { return std::string(); }

    Option &asShortName(char c) { return *this; }
    char shortName() const { return 0; }

    Option &required() { return *this; }
    bool isRequired() const { return false; }

    Option &withDescription(std::string description) { return *this; }
    std::string description() const { return std::string(); }

    template<typename T>
    Option &withDefaultValue(T defaultValue) { return *this; }
    template<typename T>
    T defaultValue() const { return T(); }

    template<typename T>
    void bindTo(T &value) {}
    template<typename T>
    T *boundTo() const { return nullptr; }
};

}
