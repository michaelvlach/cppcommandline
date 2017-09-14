#include "cppcommandlinetest.h"
#include "qtestbdd.h"
#include "cppcommandline.h"


void CppCommandLineTest::OptionDefaultCtor()
{
    SCENARIO("Option default constructor")
    cppcommandline::Option option;
    QCOMPARE(option.isPositional(), true);
    QCOMPARE(option.longName(), std::string());
    QCOMPARE(option.shortName(), std::string());
    QCOMPARE(option.description(), std::string());
    QCOMPARE(option.isRequired(), false);
    QCOMPARE(option.defaultValue<std::string>(), std::string());
    QVERIFY(option.boundValue<std::string>() == nullptr);
}

void CppCommandLineTest::OptionLongNameCtor()
{
    {
    SCENARIO("Option long name constructor")
    cppcommandline::Option option("longName1");
    QCOMPARE(option.longName(), std::string("longName1"));
    }

    {
    SCENARIO("Option invalid long name")
    QVERIFY_EXCEPTION_THROWN(cppcommandline::Option option("long.name"), std::logic_error);
    }
}

void CppCommandLineTest::OptionMoveCtor()
{
    SCENARIO("Option move ctor")
    cppcommandline::Option option(cppcommandline::Option("longName"));
    QCOMPARE(option.longName(), std::string("longName"));
}

void CppCommandLineTest::OptionMoveAssignment()
{
    SCENARIO("Option move assignment")
    cppcommandline::Option option = cppcommandline::Option("longName");
    QCOMPARE(option.longName(), std::string("longName"));
}

void CppCommandLineTest::positional()
{
    {
    SCENARIO("Option is positional without any names")
    cppcommandline::Option option;
    QCOMPARE(option.isPositional(), true);
    }

    {
    SCENARIO("Option is not positional if it has a name")
    cppcommandline::Option option("longName");
    QCOMPARE(option.isPositional(), false);
    }
}

void CppCommandLineTest::shortName()
{
    {
    SCENARIO("Option short name")
    cppcommandline::Option option = std::move(cppcommandline::Option("longName1").asShortName("o"));
    QCOMPARE(option.shortName(), std::string("o"));
    }

    {
    SCENARIO("Option invalid short name")
    QVERIFY_EXCEPTION_THROWN(cppcommandline::Option("longName").asShortName("o1"), std::logic_error);
    }
}

void CppCommandLineTest::description()
{
    SCENARIO("Option description")
    cppcommandline::Option option = std::move(cppcommandline::Option("longName").withDescription("My fancty description"));
    QCOMPARE(option.description(), std::string("My fancty description"));
}

void CppCommandLineTest::required()
{
    {
    SCENARIO("Option required")
    cppcommandline::Option option = std::move(cppcommandline::Option("longName").required());
    QCOMPARE(option.isRequired(), true);
    }

    {
    SCENARIO("Option with default value cannot be required")
    QVERIFY_EXCEPTION_THROWN(cppcommandline::Option("longName").withDefaultValue(10).required(), std::logic_error);
    }
}

void CppCommandLineTest::defaultValue()
{
    {
    SCENARIO("Option default value")
    cppcommandline::Option option = std::move(cppcommandline::Option("longName").withDefaultValue(std::string("option")));
    QCOMPARE(option.defaultValue<std::string>(), std::string("option"));
    }

    {
    SCENARIO("Option default value cannot be specified for required options")
    QVERIFY_EXCEPTION_THROWN(cppcommandline::Option("longName").required().withDefaultValue(std::string("option")), std::logic_error);
    }

    {
    SCENARIO("Option with default value cannot return different type")
    cppcommandline::Option option = std::move(cppcommandline::Option("longName").withDefaultValue(std::string("option")));
    QVERIFY_EXCEPTION_THROWN(option.defaultValue<int>(), std::logic_error);
    }
}

void CppCommandLineTest::boundValue()
{
    {
    SCENARIO("Option bound value")
    std::string boundValue;
    cppcommandline::Option option;
    option.bindTo(boundValue);
    QCOMPARE(option.boundValue<std::string>(), &boundValue);
    }

    {
    SCENARIO("Option bound value with default value")
    std::string boundValue;
    cppcommandline::Option option = std::move(cppcommandline::Option().withDefaultValue(std::string("filename")));
    option.bindTo(boundValue);
    QCOMPARE(option.boundValue<std::string>(), &boundValue);
    QCOMPARE(boundValue, std::string("filename"));
    }

    {
    SCENARIO("Option bound value with differently typed default value is not possible")
    std::string boundValue;
    cppcommandline::Option option = std::move(cppcommandline::Option().withDefaultValue(10));
    QVERIFY_EXCEPTION_THROWN(option.bindTo(boundValue), std::logic_error);
    }
}

void CppCommandLineTest::match()
{
    {
    SCENARIO("Option match separate option and value")
    std::vector<std::string> arguments{"--longName", "value"};
    std::string value;
    cppcommandline::Option option("longName");
    option.bindTo(value);
    QCOMPARE(option.match(arguments.cbegin(), arguments.cend()), arguments.cend());
    QCOMPARE(std::string("value"), value);
    }

    {
    SCENARIO("Option match joined option and value with =")
    std::vector<std::string> arguments{"--longName=value"};
    std::string value;
    cppcommandline::Option option("longName");
    option.bindTo(value);
    QCOMPARE(option.match(arguments.cbegin(), arguments.cend()), arguments.cend());
    QCOMPARE(std::string("value"), value);
    }

    {
    SCENARIO("Option match positional boolean argument")
    std::vector<std::string> arguments{"positional"};
    bool value;
    cppcommandline::Option option;
    option.bindTo(value);
    QCOMPARE(option.match(arguments.cbegin(), arguments.cend()), arguments.cend());
    QCOMPARE(true, value);
    }

    {
    SCENARIO("Option match positional argument with value")
    std::vector<std::string> arguments{"value"};
    std::string value;
    cppcommandline::Option option;
    option.bindTo(value);
    QCOMPARE(option.match(arguments.cbegin(), arguments.cend()), arguments.cend());
    QCOMPARE(std::string("value"), value);
    }

    {
    SCENARIO("Option match `int` argument")
    std::vector<std::string> arguments{"10"};
    int value = 0;
    cppcommandline::Option option;
    option.bindTo(value);
    QCOMPARE(option.match(arguments.cbegin(), arguments.cend()), arguments.cend());
    QCOMPARE(10, value);
    }

    {
    SCENARIO("Option match negative `int` argument")
    std::vector<std::string> arguments{"-10"};
    int value = 0;
    cppcommandline::Option option;
    option.bindTo(value);
    QCOMPARE(option.match(arguments.cbegin(), arguments.cend()), arguments.cend());
    QCOMPARE(-10, value);
    }

    {
    SCENARIO("Option match `double` argument")
    std::vector<std::string> arguments{"5.5"};
    double value = 0;
    cppcommandline::Option option;
    option.bindTo(value);
    QCOMPARE(option.match(arguments.cbegin(), arguments.cend()), arguments.cend());
    QCOMPARE(double(5.5), value);
    }

    {
    SCENARIO("Option match negative `double` argument")
    std::vector<std::string> arguments{"-5.5"};
    double value = 0;
    cppcommandline::Option option;
    option.bindTo(value);
    QCOMPARE(option.match(arguments.cbegin(), arguments.cend()), arguments.cend());
    QCOMPARE(double(-5.5), value);
    }

    {
    SCENARIO("Option match `long long` argument")
    std::vector<std::string> arguments{"999999999999"};
    qint64 value = 0;
    cppcommandline::Option option;
    option.bindTo(value);
    QCOMPARE(option.match(arguments.cbegin(), arguments.cend()), arguments.cend());
    QCOMPARE(value, qint64(999999999999));
    }

    {
    SCENARIO("Option match negative `long long` argument")
    std::vector<std::string> arguments{"-999999999999"};
    qint64 value = 0;
    cppcommandline::Option option;
    option.bindTo(value);
    QCOMPARE(option.match(arguments.cbegin(), arguments.cend()), arguments.cend());
    QCOMPARE(value, qint64(-999999999999));
    }
}

void CppCommandLineTest::ParserOption()
{
    SCENARIO("Parser returns a default constructed option")
    cppcommandline::Parser parser;
    QCOMPARE(parser.option(), cppcommandline::Option());
}

void CppCommandLineTest::ParserOptionLongName()
{
    SCENARIO("Parser returns a default constructed option")
    cppcommandline::Parser parser;
    QCOMPARE(parser.option("longName"), cppcommandline::Option("longName"));
}

void CppCommandLineTest::parse()
{
    {
    SCENARIO("Single positional option")
    std::vector<const char*> args{"./app.exe", "value"};
    cppcommandline::Parser parser;
    std::string value;
    parser.option().bindTo(value);
    parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data()));
    QCOMPARE(value, std::string("value"));
    QCOMPARE(parser.command(), std::string("./app.exe"));
    QCOMPARE(parser.applicationName(), std::string("app"));
    }

    {
    SCENARIO("Multiple positional options")
    std::vector<const char*> args{"./app", "value", "-10", "5.5"};
    cppcommandline::Parser parser;
    std::string value;
    int iValue = 0;
    double dValue = 0.0;
    parser.option().bindTo(iValue);
    parser.option().bindTo(dValue);
    parser.option().bindTo(value);
    parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data()));
    QCOMPARE(value, std::string("value"));
    QCOMPARE(iValue, -10);
    QCOMPARE(dValue, 5.5);
    QCOMPARE(parser.command(), std::string("./app"));
    QCOMPARE(parser.applicationName(), std::string("app"));
    }

    {
    SCENARIO("Single long name option")
    std::vector<const char*> args{"./app", "--value"};
    cppcommandline::Parser parser;
    bool value;
    parser.option("value").bindTo(value);
    parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data()));
    QCOMPARE(value, true);
    }

    {
    SCENARIO("Multiple long name options")
    std::vector<const char*> args{"./app", "--value", "--option=file", "--yetanother", "10"};
    cppcommandline::Parser parser;
    bool value;
    std::string option;
    int another = 0;
    parser.option("value").bindTo(value);
    parser.option("option").bindTo(option);
    parser.option("yetanother").bindTo(another);
    parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data()));
    QCOMPARE(value, true);
    QCOMPARE(option, std::string("file"));
    QCOMPARE(another, 10);
    }

    {
    SCENARIO("Single short name option")
    std::vector<const char*> args{"./app", "-v"};
    cppcommandline::Parser parser;
    bool value;
    parser.option("value").asShortName("v").bindTo(value);
    parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data()));
    QCOMPARE(value, true);
    }

    {
    SCENARIO("Multiple short name options")
    std::vector<const char*> args{"./app", "-v", "-o=file", "-y", "10"};
    cppcommandline::Parser parser;
    bool value;
    std::string option;
    int another = 0;
    parser.option("value").asShortName("v").bindTo(value);
    parser.option("option").asShortName("o").bindTo(option);
    parser.option("yetanother").asShortName("y").bindTo(another);
    parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data()));
    QCOMPARE(value, true);
    QCOMPARE(option, std::string("file"));
    QCOMPARE(another, 10);
    }

    {
    SCENARIO("Mixed options")
    std::vector<const char*> args{"./app", "-v", "-o=file", "--yetanother", "10", "somefile"};
    cppcommandline::Parser parser;
    bool value;
    std::string option;
    int another = 0;
    std::string positional;
    parser.option("value").asShortName("v").bindTo(value);
    parser.option("option").asShortName("o").bindTo(option);
    parser.option("yetanother").bindTo(another);
    parser.option().bindTo(positional);
    parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data()));
    QCOMPARE(value, true);
    QCOMPARE(option, std::string("file"));
    QCOMPARE(another, 10);
    QCOMPARE(positional, std::string("somefile"));
    QVERIFY(!parser.helpDisplayed());
    }
}

void CppCommandLineTest::parseFailed()
{
    {
    SCENARIO("Unmatched argument")
    std::vector<const char*> args{"./app", "-v"};
    QVERIFY_EXCEPTION_THROWN(cppcommandline::Parser().parse(static_cast<int>(args.size()), const_cast<char**>(args.data())), std::logic_error);
    }

    {
    SCENARIO("Unmatched required option")
    std::vector<const char*> args{"./app", "-v"};
    cppcommandline::Parser parser;
    parser.option("longName").asShortName("v");
    parser.option().required();
    QVERIFY_EXCEPTION_THROWN(parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data())), std::logic_error);
    }

    {
    SCENARIO("Type mismatch")
    std::vector<const char*> args{"./app", "-v=hello"};
    cppcommandline::Parser parser;
    int value = 0;
    parser.option("value").asShortName("v").bindTo(value);
    QVERIFY_EXCEPTION_THROWN(parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data())), std::logic_error);
    }

    {
    SCENARIO("Missing value")
    std::vector<const char*> args{"./app", "-v"};
    cppcommandline::Parser parser;
    int value = 0;
    parser.option("value").asShortName("v").bindTo(value);
    QVERIFY_EXCEPTION_THROWN(parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data())), std::logic_error);
    }
}

void CppCommandLineTest::help()
{

    {
    SCENARIO("Unmatched required option")
    std::vector<const char*> args{"./app", "-h"};
    cppcommandline::Parser parser;
    parser.parse(static_cast<int>(args.size()), const_cast<char**>(args.data()));
    QVERIFY(parser.helpDisplayed());
    }

}

QTEST_APPLESS_MAIN(CppCommandLineTest)
