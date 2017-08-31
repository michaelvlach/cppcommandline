#pragma once

#include <QObject>
#include <memory>

class CppCommandLineTest : public QObject
{
    Q_OBJECT
private slots:
    void OptionDefaultCtor();
    void OptionLongNameCtor();
    void OptionMoveCtor();
    void OptionMoveAssignment();
    void positional();
    void shortName();
    void description();
    void required();
    void defaultValue();
    void boundValue();
    void match();
    void ParserOption();
    void ParserOptionLongName();
    void parse();
    void parseFailed();

private:
    std::unique_ptr<char**, void(*)(char**)> createArguments(std::vector<std::string> arguments);
};
