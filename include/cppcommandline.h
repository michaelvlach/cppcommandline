#pragma once

#include <vector>
#include <string>
#include <variant>
#include <stdexcept>
#include <regex>

namespace cppcommandline
{

class Option
{
public:
    explicit Option(std::string longName) :
        mLongName(longName)
    {
        std::regex regex("^[a-zA-Z\\d]$");
        if(!std::regex_match(mLongName, regex))
            throw std::logic_error("The name '" + mLongName + "' is not valid name of the option.");
    }

    ~Option()
    {
    }

    Option &asShortName(std::string shortName)
    {
        mShortName = shortName;
        std::regex regex("^[a-zA-Z\\d]$");
        if(!std::regex_match(mLongName, regex))
            throw std::logic_error("The name '" + mShortName + "' is not valid short name of the option.");
        return *this;
    }

    Option &required()
    {
        if(mType == Type::Undefined)
            mRequired = true;
        else
            throw std::logic_error("The option '" + mLongName + "' has default value and cannot be set as required.");
        return *this;
    }

    template<typename T>
    Option &withDefaultParameter(T defaultValue)
    {
        if(mRequired)
            throw std::logic_error("The option '" + mLongName + "' is set as required and cannot have default value assigned.");
        else
//            mDefaultValue = defaultValue; //TODO
        return *this;
    }

    template<typename T>
    Option &bindTo(T &value)
    {
        mValue.emplace(&value);
        return *this;
    }

private:
    union DefaultValue
    {
        DefaultValue()
        {
        }

        ~DefaultValue()
        {
        }

        std::string s;
        int i;
        long long ll;
        double d;
        bool b = false;
    };

    union Value
    {
        std::string *s;
        int *i;
        long long *ll;
        double *d;
        bool *b = nullptr;
    };

    enum class Type
    {
        Undefined,
        String,
        Integer,
        LongLong,
        Double,
        Bool
    };

    std::string mLongName;
    std::string mShortName;
    DefaultValue mDefaultValue;
    Value mValue;
    Type mType = Type::Undefined;
    bool mRequired = false;
};

class Parser
{
public:
    Option &option(std::string longName)
    {
        mOptions.emplace_back(Option(longName));
        return mOptions.back();
    }

    void parse(int argc, char **argv)
    {
        mArgc = argc;
        mArgv = argv;
    }

private:
    int mArgc = 0;
    char **mArgv = nullptr;
    std::vector<Option> mOptions;
};

}
