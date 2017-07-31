import qbs

Project
{
    Product
    {
        files: [ "include/cppcommandline.h" ]
    }

    QtApplication
    {
        name: "cppcommandlinetest"
        Depends { name: "Qt.testlib" }
        cpp.cxxLanguageVersion: "c++11"
        cpp.includePaths: [ "include", "test" ]
        files: [ "test/*" ]
    }
}
