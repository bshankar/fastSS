TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 \
    -march=native \
    -Wno-write-strings


SOURCES += src/main.cpp \
    src/matrix.cpp \
    src/solve.cpp \
    src/generate.cpp

HEADERS += \
    src/matrix.h \
    src/solve.h \
    src/generate.h \
    src/data.h

