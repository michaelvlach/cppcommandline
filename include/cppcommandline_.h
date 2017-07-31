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
    enum class Match
    {
        None,
        Argument,
        ArgumentAndValue
    };

    Option() :
        d(std::unique_ptr<OptionPrivate>(new OptionPrivate))
    {

    }

    explicit Option(std::string longName) :
        d(std::unique_ptr<OptionPrivate>(new OptionPrivate))
    {
        if(!isLongName(longName))
            throw std::logic_error("The name '" + longName + "' is not a valid option name.");
        else
            d->longName = longName;
    }

    Option(Option &&option) :
        d(option.d.release())
    {

    }

    Option(const Option &option) = delete;

    Option &operator=(Option &&option)
    {
        d.reset(option.d.release());
        return *this;
    }

    Option &asShortName(std::string shortName)
    {

        if(d->longName.empty())
            throw std::logic_error("Short name cannot be defined for positional arguments.");
        else if(!isShortName(shortName))
            throw std::logic_error("The name '" + shortName + "' is not a valid short option name.");
        else
            d->shortName = shortName;
        return *this;
    }

    Option &required()
    {
        if(d->type == Type::Undefined)
            d->required = true;
        else
            throw std::logic_error("The option " + getName() + " has default value and cannot be set as required.");
        return *this;
    }

    template<typename T>
    Option &withDefaultValue(T defaultValue)
    {
        if(d->required)
            throw std::logic_error("The option " + getName() + " is set as required and cannot have default value assigned.");
        else
        {
            setDefault(defaultValue);
            d->type = getType<T>();
        }
        return *this;
    }

    template<typename T>
    void bindTo(T &value)
    {
        if(d->type == Type::Undefined)
            d->type = getType<T>();
        else if(!defaultTypeCompatibleWithBoundType(d->type, getType<T>()))
            throw std::logic_error("The option " + getName() + " has default value set with incompatible type (" + getTypeAsString(d->type) + ") to the one it is being bound to (" + getTypeAsString(getType<T>()) + ")" );
        setValueBinding(&value);
    }

    Match match(std::string argument, std::string next = std::string())
    {
        Match result = Match::None;
        KeyValue keyValue = getKeyValue(argument);

        if(keyValue.key.empty()) //either positional or next is a value
        {
            if(isLongNameArgument(argument))
            {

            }
            else if(isShortNameArgument(argument))
            {

            }
            else //positional, next is not part of this resolution
            {
                if(d->type == Type::Bool) //next is not part of this resolution
                {

                }
                else
                {

                }
            }
        }
        else //next is not part of this resolution
        {
            if(isLongNameArgument(keyValue.key))
            {

            }
            else if(isShortNameArgument(keyValue.key))
            {

            }
        }

        return result;
    }

private:
    union DefaultValue
    {
        int i;
        long long l;
        double d;
        bool b = false;
    };

    union ValueBinding
    {
        std::string *s;
        int *i;
        long long *l;
        double *d;
        bool *b = nullptr;
    };

    struct KeyValue
    {
        std::string key;
        std::string value;
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

    struct OptionPrivate
    {
        std::string longName;
        std::string shortName;
        std::string defaultStringValue;
        Option::DefaultValue defaultValue;
        Option::ValueBinding valueBinding;
        Option::Type type = Type::Undefined;
        bool required = false;
    };

    template<typename T> void setDefault(T);
    template<typename T> void setValueBinding(T*);
    template<typename T> Type getType() const;

    std::string getName() const
    {
        return d->longName.empty() ? "[Positional]" : "'" + d->longName + "'";
    }

    std::string getTypeAsString(Type type) const
    {
        switch(type)
        {
        case Type::Undefined: return "Undefined";
        case Type::Bool: return "bool";
        case Type::Double: return "double";
        case Type::Integer: return "int";
        case Type::LongLong: return "long long";
        case Type::String: return "string";
        }
    }

    KeyValue getKeyValue(std::string argument) const
    {
        KeyValue keyValue;
        if(std::regex_match(argument, std::regex("^(--|-)[a-zA-Z\\d]=.*")))
        {
            size_t pos = argument.find_first_of('=') + 1;
            keyValue.key = argument.substr(0, pos);
            keyValue.value = argument.substr(pos, argument.size() - pos);
        }
        return keyValue;
    }

    bool defaultTypeCompatibleWithBoundType(Type defaultType, Type boundType) const
    {
        return defaultType != Type::Undefined && boundType != Type::Undefined && (defaultType == boundType || (defaultType == Type::Integer && boundType == Type::LongLong));
    }

    bool isLongNameArgument(std::string argument) const
    {
        return std::regex_match(argument, std::regex("^--[a-zA-Z\\d]+$"));
    }

    bool isShortNameArgument(std::string argument) const
    {
        return std::regex_match(argument, std::regex("^-[a-zA-Z\\d]$"));
    }

    bool isInteger(std::string argument) const
    {
        return isNumber(argument) && (std::stoi(argument) <= std::numeric_limits<int>::min() || std::stoll(argument) <= std::numeric_limits<int>::max());
    }

    bool isLongLong(std::string argument) const
    {
        return isNumber(argument) && (std::stoll(argument) < std::numeric_limits<int>::min() || std::stoll(argument) > std::numeric_limits<int>::max());
    }

    bool isDouble(std::string argument) const
    {
        return std::regex_match(argument, std::regex("^(-|\\d)\\d+(.|,)\\d+$"));
    }

    bool isNumber(std::string argument) const
    {
        return std::regex_match(argument, std::regex("^(-|\\d)\\d+$"));
    }

    bool isLongName(std::string longName) const
    {
        return std::regex_match(longName, std::regex("^[a-zA-Z\\d]+$"));
    }

    bool isShortName(std::string shortName) const
    {
        return std::regex_match(shortName, std::regex("^[a-zA-Z\\d]$"));
    }

    std::unique_ptr<OptionPrivate> d;
};

template<> void Option::setDefault(std::string defaultValue) { d->defaultStringValue = defaultValue; }
template<> void Option::setDefault(int defaultValue) { d->defaultValue.i = defaultValue; }
template<> void Option::setDefault(long long defaultValue) { d->defaultValue.l = defaultValue; }
template<> void Option::setDefault(double defaultValue) { d->defaultValue.d = defaultValue; }
template<> void Option::setDefault(bool defaultValue) { d->defaultValue.b = defaultValue; }
template<> void Option::setValueBinding(std::string *binding) { d->valueBinding.s = binding; }
template<> void Option::setValueBinding(int *binding) { d->valueBinding.i = binding; }
template<> void Option::setValueBinding(long long *binding) { d->valueBinding.l = binding; }
template<> void Option::setValueBinding(double *binding) { d->valueBinding.d = binding; }
template<> void Option::setValueBinding(bool *binding) { d->valueBinding.b = binding; }
template<> Option::Type Option::getType<std::string>() const { return Type::String; }
template<> Option::Type Option::getType<int>() const { return Type::Integer; }
template<> Option::Type Option::getType<long long>() const { return Type::LongLong; }
template<> Option::Type Option::getType<bool>() const { return Type::Bool; }
template<> Option::Type Option::getType<double>() const { return Type::Double; }

class Parser
{
public:
    Option &option()
    {
        mOptions.emplace_back(Option());
        return mOptions.back();
    }

    Option &option(std::string longName)
    {
        mOptions.emplace_back(Option(longName));
        return mOptions.back();
    }

    void parse(int argc, char **argv)
    {
        mArgc = argc;
        mArgv = argv;

        for(int i = 0; i < mArgc; ++i)
        {

        }
    }

private:
    int mArgc = 0;
    char **mArgv = nullptr;
    std::vector<Option> mOptions;
};

}
