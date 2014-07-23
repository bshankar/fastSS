TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 \
    -march=native

SOURCES += main.cpp \
    matrix.cpp \
    solve.cpp

HEADERS += \
    matrix.h \
    solve.h

