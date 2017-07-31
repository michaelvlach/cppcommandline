#pragma once

#include <QTest>

namespace cppcommandline
{
    class Option;
}

class CppCommandLineTest : public QObject
{
    Q_OBJECT
private slots:
    void Option_positional();
    void Option_validLongName_data();
    void Option_validLongName();
    void Option_invalidLongName_data();
    void Option_invalidLongName();
    void Option_validShortName_data();
    void Option_validShortName();
    void Option_invalidShortName_data();
    void Option_invalidShortName();
    void Option_positionalShortName();
    void Option_positionalRequired();
    void Option_namedRequired();
    void Option_requiredWithDefaultValue();
    void Option_positionalWithDescription();
    void Option_namedWithDescription();
    void Option_positionalWithDefaultValue_data();
    void Option_positionalWithDefaultValue();
    void Option_namedWithDefaultValue_data();
    void Option_namedWithDefaultValue();
    void Option_defaultValueWithRequired();
    void Option_bindTo_data();
    void Option_bindTo();

private:
    void prepareDataTypes();

    template<typename T> void verifyDefaultValue(cppcommandline::Option &&option, T defaultValue)
    {
        option.withDefaultValue(defaultValue);
        QCOMPARE(option.defaultValue<T>(), defaultValue);
    }

    template<typename T> void verifyBinding()
    {
        T value;
        Option &option = Option();
        option.bindTo<T>(value);
        QCOMPARE(option.boundTo<T>(), &value);
    }
};
