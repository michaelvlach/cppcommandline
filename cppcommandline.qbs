import qbs

Project
{
    Product
    {
        files: [ "include/cppcommandline.h" ]
    }

    CppApplication
    {

        name: "cppcommandlineexample"
        cpp.includePaths: [ "include", "example" ]
        cpp.cxxLanguageVersion: "c++11"
        files: [ "example/*" ]
    }

    QtApplication
    {
        Depends { name: "Qt.testlib" }
        name: "cppcommandlinetest"
        cpp.includePaths: [ "include", "test" ]
        cpp.cxxLanguageVersion: "c++11"
        files: [ "test/*" ]
    }
}
