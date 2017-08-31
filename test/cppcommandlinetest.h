#pragma once

#include <QObject>

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
};
