TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS_DEBUG = -std=c++11 -O0 -g
QMAKE_CFLAGS_RELEASE = -std=c++11 -O3 \
    -march=native -Wall -DNDEBUG

QMAKE_CXXFLAGS_DEBUG = -std=c++11 -O0 -g
QMAKE_CXXFLAGS_RELEASE = -std=c++11 -O3 \
    -march=native -Wall -DNDEBUG


SOURCES += src/main.cpp \
    src/matrix.cpp \
    src/solve.cpp \
    src/generate.cpp

HEADERS += \
    src/matrix.h \
    src/solve.h \
    src/generate.h \
    src/data.h

