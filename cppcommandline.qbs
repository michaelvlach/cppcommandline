import qbs

Project
{
    Product
    {
        files: [ "include/cppcommandline.h" ]
    }

    QtApplication
    {

        name: "cppcommandlineexample"
        cpp.includePaths: [ "include", "example" ]
        files: [ "example/*" ]
    }

    QtApplication
    {
        Depends { name: "Qt.testlib" }
        name: "cppcommandlinetest"
        cpp.includePaths: [ "include", "test" ]
        files: [ "test/*" ]
    }
}
