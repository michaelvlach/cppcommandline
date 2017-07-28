import qbs

Project
{
    CppApplication
    {
        Depends { name: "gtest" }
        cpp.includePaths: "include"
        files: [ "test/cppcommandlinetest.cpp", "include/cppcommandline.h" ]
    }
}
