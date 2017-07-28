SOURCES = test/cppcommandlinetest.cpp
HEADERS = include/cppcommandline.h


win32: LIBS += -L$$PWD/../googletest/BIN/lib/ -lgtest

INCLUDEPATH += $$PWD/../googletest/BIN/include
DEPENDPATH += $$PWD/../googletest/BIN/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../googletest/BIN/lib/gtest.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../googletest/BIN/lib/libgtest.a

win32: LIBS += -L$$PWD/../googletest/BIN/lib/ -lgtest_main

INCLUDEPATH += $$PWD/../googletest/BIN/include
DEPENDPATH += $$PWD/../googletest/BIN/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../googletest/BIN/lib/gtest_main.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../googletest/BIN/lib/libgtest_main.a
