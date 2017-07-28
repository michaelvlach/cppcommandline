import qbs

Module
{
    Depends { name: "cpp" }
    cpp.includePaths: [ "../googletest/BIN/include" ]
    cpp.libraryPaths: [ "../googletest/BIN/lib" ]
    cpp.staticLibraries: [ "gtest.lib", "gtest_main.lib" ]
}
