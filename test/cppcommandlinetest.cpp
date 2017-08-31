#include "cppcommandlinetest.h"
#include "qtestbdd.h"
#include "cppcommandline.h"


void CppCommandLineTest::OptionDefaultCtor()
{
    SCENARIO("Option default constructor")
    cppcommandline::Option option;
    QCOMPARE(true, option.isPositional());
    QCOMPARE(std::string(), option.longName());
    QCOMPARE(std::string(), option.shortName());
    QCOMPARE(std::string(), option.description());
    QCOMPARE(false, option.isRequired());
    QCOMPARE(std::string(), option.defaultValue<std::string>());
    QCOMPARE(nullptr, option.boundValue<std::string>());
}

void CppCommandLineTest::OptionLongNameCtor()
{
    {
    SCENARIO("Option long name constructor")
    cppcommandline::Option option("longName1");
    QCOMPARE(std::string("longName1"), option.longName());
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
    QCOMPARE(std::string("longName"), option.longName());
}

void CppCommandLineTest::OptionMoveAssignment()
{
    SCENARIO("Option move assignment")
    cppcommandline::Option option = cppcommandline::Option("longName");
    QCOMPARE(std::string("longName"), option.longName());
}

void CppCommandLineTest::positional()
{
    {
    SCENARIO("Option is positional without any names")
    cppcommandline::Option option;
    QCOMPARE(true, option.isPositional());
    }

    {
    SCENARIO("Option is not positional if it has a name")
    cppcommandline::Option option("longName");
    QCOMPARE(false, option.isPositional());
    }
}

void CppCommandLineTest::shortName()
{
    {
    SCENARIO("Option short name")
    cppcommandline::Option option = std::move(cppcommandline::Option("longName1").asShortName("o"));
    QCOMPARE(std::string("o"), option.shortName());
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
    QCOMPARE(std::string("My fancty description"), option.description());
}

void CppCommandLineTest::required()
{
    {
    SCENARIO("Option required")
    cppcommandline::Option option = std::move(cppcommandline::Option("longName").required());
    QCOMPARE(true, option.isRequired());
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
    QCOMPARE(std::string("option"), option.defaultValue<std::string>());
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
    QCOMPARE(&boundValue, option.boundValue<std::string>());
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
    SCENARIO("Option match")
    std::vector<std::string> arguments{"longName", "value"};
    std::string value;
    cppcommandline::Option option("longName");
    option.bindTo(value);
    QCOMPARE(arguments.cend(), option.match(arguments.cbegin(), arguments.cend()));
    }
}

void CppCommandLineTest::ParserOption()
{
    SCENARIO("Parser returns a default constructed option")
    cppcommandline::Parser parser;
    QCOMPARE(cppcommandline::Option(), parser.option());
}

void CppCommandLineTest::ParserOptionLongName()
{
    SCENARIO("Parser returns a default constructed option")
    cppcommandline::Parser parser;
    QCOMPARE(cppcommandline::Option("longName"), parser.option("longName"));
}

void CppCommandLineTest::parse()
{

}

QTEST_APPLESS_MAIN(CppCommandLineTest)
