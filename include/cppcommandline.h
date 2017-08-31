#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <regex>
#include <memory>
#include <iostream>

namespace cppcommandline
{

class Option
{
public:
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
        d.reset(option.d.release()); return *this;
    }

    bool operator==(const Option &other) const
    {
        return d->defaultStringValue == other.d->defaultStringValue
            && d->description == other.d->description
            && d->longName == other.d->longName
            && d->required == other.d->required
            && d->shortName == other.d->shortName
            && d->type == other.d->type;
        //TODO: Compare the union values.
    }

    bool isPositional() const
    {
        return d->longName.empty();
    }

    std::string longName() const
    {
        return d->longName;
    }

    std::string shortName() const
    {
        return d->shortName;
    }

    std::string description() const
    {
        return d->description;
    }

    bool isRequired() const
    {
        return d->required;
    }

    template<typename T>
    T defaultValue() const
    {
        T val;

        if(d->type != Type::Undefined)
        {
            if(getType<T>() == d->type)
                val = getDefaultValue<T>();
            else
                throw std::logic_error("Type mismatch: requesting '" + getTypeAsString(getType<T>()) + "' but the set type is '" + getTypeAsString(d->type) + "'");
        }

        return val;
    }

    template<typename T>
    T *boundValue() const
    {
        T *val = nullptr;

        if(d->type != Type::Undefined)
        {
            if(d->type != Type::Undefined)
            {
                if(getType<T>() == d->type)
                    val = getBoundValue<T>();
                else
                    throw std::logic_error("Type mismatch: requesting '" + getTypeAsString(getType<T>()) + "' but the set type is '" + getTypeAsString(d->type) + "'");
            }
        }

        return val;
    }

    Option &asShortName(std::string shortName)
    {

        if(d->longName.empty())
            throw std::logic_error("Short name cannot be defined for positional arguments.");
        else if(!isShortName(shortName))
            throw std::logic_error("The name '" + shortName + "' is not a valid short option name.");
        else if(d->shortName.empty())
            d->shortName = shortName;
        else
            throw std::logic_error("The option '" + shortName + "' already has a short name + '" + d->shortName + "'.");
        return *this;
    }

    Option &required()
    {
        if(!d->defaulted)
            d->required = true;
        else
            throw std::logic_error("The option " + getName() + " has default value and cannot be set as required.");
        return *this;
    }

    Option &withDescription(std::string description)
    {
        if(d->description.empty())
            d->description = description;
        else
            throw std::logic_error("The option " + getName() + " already has a description.");
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
            d->defaulted = true;
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

    std::vector<std::string>::const_iterator match(std::vector<std::string>::const_iterator argument, std::vector<std::string>::const_iterator end)
    {
        KeyValue keyValue = getKeyValue(*argument);

        if(keyValue.key.empty())
        {
            if(isPositional())
            {
                if(setValue(*argument))
                    ++argument;
            }
        }
        else if((isLongNameArgument(*argument) && keyValue.key == longName()) || (isShortNameArgument(*argument) && keyValue.key == shortName()))
        {
            if(d->type == Type::Bool)
            {
                if(setValue(*argument))
                    ++argument;
            }
            else if(keyValue.value.empty())
            {
                std::vector<std::string>::const_iterator it = argument+1;

                if(it == end)
                    throw(std::logic_error("Missing value for option '" + longName() + "'"));
                else if(setValue(*it))
                    argument += 2;
            }
            else if(setValue(keyValue.value))
                ++argument;
        }

        return argument;
    }

private:
    enum class Type
    {
        Undefined,
        String,
        Integer,
        LongLong,
        Double,
        Bool
    };

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

    struct OptionPrivate
    {
        std::string longName;
        std::string shortName;
        std::string defaultStringValue;
        std::string description;
        Option::DefaultValue defaultValue;
        Option::ValueBinding valueBinding;
        Option::Type type = Type::Undefined;
        bool required = false;
        bool defaulted = false;
    };

    template<typename T> T *getBoundValue() const;
    template<typename T> T getDefaultValue() const;
    template<typename T> void setDefault(T);
    template<typename T> void setValueBinding(T*);
    template<typename T> Type getType() const;

    bool setValue(std::string value)
    {
        bool result = true;

        class ValueParseError : public std::logic_error
        {
        public:
            ValueParseError() :
                std::logic_error("...")
            {

            }
        };

        try
        {
            switch(d->type)
            {
            case Type::Bool:
                *d->valueBinding.b = true;
                break;
            case Type::Double:
                if(isDouble(value))
                    *d->valueBinding.d = std::stod(value);
                else
                    throw(ValueParseError());
                break;
            case Type::Integer:
                if(isInteger(value))
                    *d->valueBinding.i = std::stoi(value);
                else
                    throw(ValueParseError());
                break;
            case Type::LongLong:
                if(isLongLong(value))
                    *d->valueBinding.l = std::stoll(value);
                else
                    throw(ValueParseError());
                break;
            case Type::String:
                *d->valueBinding.s = value;
                break;
            case Type::Undefined:
                throw(std::logic_error("Bind value undefined for option '" + longName() + "'"));
                break;
            }
        }
        catch(ValueParseError&)
        {
            result = false;
        }

        return result;
    }

    std::string getName() const
    {
        return d->longName.empty() ? "[Positional]" : "'" + d->longName + "'";
    }

    std::string getTypeAsString(Type type) const
    {
        std::string val = "Undefined";

        switch(type)
        {
        case Type::Bool: val = "bool"; break;
        case Type::Double: val = "double"; break;
        case Type::Integer: val = "int"; break;
        case Type::LongLong: val = "long long"; break;
        case Type::String: val = "string"; break;
        case Type::Undefined: break;
        }

        return val;
    }

    KeyValue getKeyValue(std::string argument) const
    {
        KeyValue keyValue;
        std::cmatch m;
        if(std::regex_match(argument.c_str(), m, std::regex("^(--|-)([a-zA-Z][a-zA-Z\\d]*)((=(.*))|)")))
        {
            keyValue.key = m.size() > 2 ? std::string(m[2]) : "";
            keyValue.value = m.size() > 4 ? std::string(m[5]) : "";
        }
        else
            keyValue.value = argument;
        return keyValue;
    }

    bool defaultTypeCompatibleWithBoundType(Type defaultType, Type boundType) const
    {
        return defaultType != Type::Undefined && boundType != Type::Undefined && (defaultType == boundType || (defaultType == Type::Integer && boundType == Type::LongLong));
    }

    bool isLongNameArgument(std::string argument) const
    {
        return std::regex_match(argument, std::regex("^--[a-zA-Z][a-zA-Z\\d]*.*$"));
    }

    bool isShortNameArgument(std::string argument) const
    {
        return std::regex_match(argument, std::regex("^-[a-zA-Z].*$"));
    }

    bool isInteger(std::string argument) const
    {
        return isNumber(argument) && (std::stoi(argument) <= std::numeric_limits<int>::min() || std::stoll(argument) <= std::numeric_limits<int>::max());
    }

    bool isLongLong(std::string argument) const
    {
        return isNumber(argument);
    }

    bool isDouble(std::string argument) const
    {
        return std::regex_match(argument, std::regex("^(-|)\\d+\\.\\d+$"));
    }

    bool isNumber(std::string argument) const
    {
        return std::regex_match(argument, std::regex("^(-|)\\d+$"));
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

template<> std::string *Option::getBoundValue() const { return d->valueBinding.s; }
template<> int *Option::getBoundValue() const { return d->valueBinding.i; }
template<> long long *Option::getBoundValue() const { return d->valueBinding.l; }
template<> double *Option::getBoundValue() const { return d->valueBinding.d; }
template<> bool *Option::getBoundValue() const { return d->valueBinding.b; }
template<> std::string Option::getDefaultValue() const { return d->defaultStringValue; }
template<> int Option::getDefaultValue() const { return d->defaultValue.i; }
template<> long long Option::getDefaultValue() const { return d->defaultValue.l; }
template<> double Option::getDefaultValue() const { return d->defaultValue.d; }
template<> bool Option::getDefaultValue() const { return d->defaultValue.b; }
template<> void Option::setDefault(std::string defaultValue) { d->defaultStringValue = defaultValue; }
template<> void Option::setDefault(int defaultValue) { d->defaultValue.i = defaultValue; }
template<> void Option::setDefault(long long defaultValue) { d->defaultValue.l = defaultValue; }
template<> void Option::setDefault(double defaultValue) { d->defaultValue.d = defaultValue; }
template<> void Option::setDefault(bool defaultValue) { d->defaultValue.b = defaultValue; }
template<> void Option::setValueBinding(std::string *binding) { d->valueBinding.s = binding; if(d->defaulted) *binding = d->defaultStringValue; }
template<> void Option::setValueBinding(int *binding) { d->valueBinding.i = binding; if(d->defaulted) *binding = d->defaultValue.i; }
template<> void Option::setValueBinding(long long *binding) { d->valueBinding.l = binding; if(d->defaulted) *binding = d->defaultValue.l; }
template<> void Option::setValueBinding(double *binding) { d->valueBinding.d = binding; if(d->defaulted) *binding = d->defaultValue.d; }
template<> void Option::setValueBinding(bool *binding) { d->valueBinding.b = binding; if(d->defaulted) *binding = d->defaultValue.b; }
template<> Option::Type Option::getType<std::string>() const { return Type::String; }
template<> Option::Type Option::getType<int>() const { return Type::Integer; }
template<> Option::Type Option::getType<long long>() const { return Type::LongLong; }
template<> Option::Type Option::getType<bool>() const { return Type::Bool; }
template<> Option::Type Option::getType<double>() const { return Type::Double; }

class Parser
{
public:
    std::string command() const
    {
        return mCommand;
    }

    std::string applicationName() const
    {
        return mAppName;
    }

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
        if(argc == 0)
            throw(std::logic_error("Missing mandatory first command line argument"));
        else
        {
            mCommand = argv[0];
            std::cmatch m;
            if(std::regex_match(mCommand.c_str(), m, std::regex("([^\\\\\\/]+$)")))
            {
                mAppName = m[1];

                if(std::regex_match(mAppName.c_str(), std::regex("\\.exe$")))
                    mAppName.erase(mAppName.size() - 4, 4);
            }
        }

        mArgs = std::vector<std::string>();

        for(int i = 1; i < argc; i++)
            mArgs.emplace_back(std::string(argv[i]));

        std::vector<Option*> options;

        for(Option &option : mOptions)
            options.emplace_back(&option);

        for(auto arg = mArgs.cbegin(); arg != mArgs.cend();)
        {
            auto start = arg;

            for(auto option = options.begin(); option != options.end();)
            {
                auto next = (*option)->match(arg, mArgs.cend());

                if(next != arg)
                {
                    option = options.erase(option);
                    arg = next;
                    break;
                }
                else
                    ++option;
            }

            if(arg == start)
                throw(std::logic_error("No option set for argument '" + *arg + "'"));
        }

        for(Option *option : options)
        {
            if(option->isRequired())
                throw(std::logic_error("Option '" + option->longName() + "' was set as required but did not match any arguments"));
        }
    }

private:
    std::string mCommand;
    std::string mAppName;
    std::vector<std::string> mArgs;
    std::vector<Option> mOptions;
};

}
