# cppcommandline

Command line parser for C++11 and later using the fluid interface.

# features

- fluid interface
- short and long names for options
- positional arguments
- value bindings
- default values
- required options
- option descriptions
- application name extraction
- automatic help
- error handling using standard exceptions

# usage

```
#include <cppcommandline.h>

int main(int argc, char **argv)
{
    cppcommandline::Parser commandLine;

    std::string filename;
    int someoption = 0;
    bool flag = false;

    commandLine.option().withDescription("A required filename").required().bindTo(filename); //positional
    commandLine.option("someoption").asShortName("s").withDefaultValue(10).withDescription("My description of someoption").bindTo(someoption);
    commandLine.option("flag").withDefaultValue(false).withDescription("My flag").bindTo(flag);
    commandLine.parse(argc, argv);
    
    return 0;
}
```
