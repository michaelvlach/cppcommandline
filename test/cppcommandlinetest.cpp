#include "cppcommandlinetest.h"
#include "cppcommandline.h"

using namespace cppcommandline;

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(Option::Type)

void CppCommandLineTest::Option_positional()
{
    Option option;
    QCOMPARE(option.longName(), std::string());
    QCOMPARE(option.isPositional(), true);
}

void CppCommandLineTest::Option_validLongName_data()
{
    QTest::addColumn<std::string>("longName");
    QTest::newRow("letters") << std::string("longname");
    QTest::newRow("LetTers") << std::string("LongName");
    QTest::newRow("number") << std::string("1");
    QTest::newRow("numbers") << std::string("123");
    QTest::newRow("alphanumeric") << std::string("Long1Name2");
}

void CppCommandLineTest::Option_validLongName()
{
    QFETCH(std::string, longName);
    Option option(longName);
    QCOMPARE(option.longName(), longName);
}

void CppCommandLineTest::Option_invalidLongName_data()
{
    QTest::addColumn<std::string>("longName");
    QTest::newRow("null") << std::string();
    QTest::newRow("empty") << std::string("");
    QTest::newRow("space") << std::string("Long Name");
    QTest::newRow("special characters") << std::string("-Long~Name'");
}

void CppCommandLineTest::Option_invalidLongName()
{
    QFETCH(std::string, longName);
    QVERIFY_EXCEPTION_THROWN(Option(longName), std::logic_error);
}

void CppCommandLineTest::Option_validShortName_data()
{
    QTest::addColumn<char>("shortName");
    QTest::newRow("letter") << 's';
    QTest::newRow("LETTER") << 'S';
    QTest::newRow("number") << '1';
}

void CppCommandLineTest::Option_validShortName()
{
    QFETCH(char, shortName);
    const Option &option = Option("LongName").asShortName(shortName);
    QCOMPARE(option.shortName(), shortName);
}

void CppCommandLineTest::Option_invalidShortName_data()
{
    QTest::addColumn<char>("shortName");
    QTest::newRow("space") << ' ';
    QTest::newRow("special character") << '-';
}

void CppCommandLineTest::Option_invalidShortName()
{
    QFETCH(char, shortName);
    QVERIFY_EXCEPTION_THROWN(Option("LongName").asShortName(shortName), std::logic_error);
}

void CppCommandLineTest::Option_positionalShortName()
{
    QVERIFY_EXCEPTION_THROWN(Option().asShortName('s'), std::logic_error);
}

void CppCommandLineTest::Option_positionalRequired()
{
    const Option &option = Option().required();
    QCOMPARE(option.isRequired(), true);
}

void CppCommandLineTest::Option_namedRequired()
{
    const Option &option = Option("LongName").asShortName('s').required();
    QCOMPARE(option.isRequired(), true);
}

void CppCommandLineTest::Option_requiredWithDefaultValue()
{
    QVERIFY_EXCEPTION_THROWN(Option().withDefaultValue("Value").required(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(Option("LongName").withDefaultValue("Value").required(), std::logic_error);
}

void CppCommandLineTest::Option_positionalWithDescription()
{
    const Option &option = Option().withDefaultValue("Some description.");
    QCOMPARE(option.description(), std::string("Some description."));
}

void CppCommandLineTest::Option_namedWithDescription()
{
    const Option &option = Option("LongName").withDescription("Some description.");
    QCOMPARE(option.description(), std::string("Some description."));
}

void CppCommandLineTest::Option_positionalWithDefaultValue_data()
{
    prepareDataTypes();
}

void CppCommandLineTest::Option_positionalWithDefaultValue()
{
    QFETCH(Option::Type, type);
    switch(type)
    {
    case Option::Type::Boolean: verifyDefaultValue<bool>(Option("LongName"), true); break;
    case Option::Type::Double: verifyDefaultValue<double>(Option("LongName"), 1.0); break;
    case Option::Type::Integer: verifyDefaultValue<int>(Option("LongName"), 1); break;
    case Option::Type::LongLong: verifyDefaultValue<long long>(Option("LongName"), 10'000'000'000); break;
    case Option::Type::String: verifyDefaultValue<std::string>(Option("LongName"), std::string("Value")); break;
    case Option::Type::Undefined: QFAIL("Undefined type"); break;
    }
}

void CppCommandLineTest::Option_namedWithDefaultValue_data()
{
    prepareDataTypes();
}

void CppCommandLineTest::Option_namedWithDefaultValue()
{
    QFETCH(Option::Type, type);
    switch(type)
    {
    case Option::Type::Boolean: verifyDefaultValue<bool>(Option("LongName"), true); break;
    case Option::Type::Double: verifyDefaultValue<double>(Option("LongName"), 1.0); break;
    case Option::Type::Integer: verifyDefaultValue<int>(Option("LongName"), 1); break;
    case Option::Type::LongLong: verifyDefaultValue<long long>(Option("LongName"), 10'000'000'000); break;
    case Option::Type::String: verifyDefaultValue<std::string>(Option("LongName"), std::string("Value")); break;
    case Option::Type::Undefined: QFAIL("Undefined type"); break;
    }
}

void CppCommandLineTest::Option_defaultValueWithRequired()
{
    QVERIFY_EXCEPTION_THROWN(Option().required().withDefaultValue("Value"), std::logic_error);
}

void CppCommandLineTest::Option_bindTo_data()
{
    prepareDataTypes();
}

void CppCommandLineTest::Option_bindTo()
{
    QFETCH(Option::Type, type);
    switch(type)
    {
    case Option::Type::Boolean: verifyBinding<bool>(); break;
    case Option::Type::Double: verifyBinding<double>(); break;
    case Option::Type::Integer: verifyBinding<int>(); break;
    case Option::Type::LongLong: verifyBinding<long long>(); break;
    case Option::Type::String: verifyBinding<std::string>(); break;
    case Option::Type::Undefined: QFAIL("Undefined type"); break;
    }
}

void CppCommandLineTest::prepareDataTypes()
{
    QTest::addColumn<Option::Type>("type");
    QTest::newRow("boolean") << Option::Type::Boolean;
    QTest::newRow("double") << Option::Type::Double;
    QTest::newRow("integer") << Option::Type::Integer;
    QTest::newRow("long long") << Option::Type::LongLong;
    QTest::newRow("string") << Option::Type::String;
}

QTEST_APPLESS_MAIN(CppCommandLineTest)
