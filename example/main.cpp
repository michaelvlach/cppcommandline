#include <cppcommandline.h>

int main(int argc, char **argv)
{
    cppcommandline::Parser commandLine;

    int someoption = 0;
    std::string filename;
    bool flag = false;

    commandLine.option().withDescription("A required filename").required().bindTo(filename);
    commandLine.option("someoption").asShortName("s").withDefaultValue(10).withDescription("My description of someoption").bindTo(someoption);
    commandLine.option("flag").withDefaultValue(false).withDescription("My flag").bindTo(flag);

    commandLine.parse(argc, argv);
    return 0;
}
