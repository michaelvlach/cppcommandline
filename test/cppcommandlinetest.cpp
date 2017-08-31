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

QTEST_APPLESS_MAIN(CppCommandLineTest)
