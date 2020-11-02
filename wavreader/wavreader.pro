#-------------------------------------------------
#
# Project created by QtCreator 2020-07-11T10:51:30
#
#-------------------------------------------------

QT       -= core gui

QMAKE_CXXFLAGS += -std=c++11
TARGET = wavreader
TEMPLATE = lib


include($$PWD/../soundsynth.pri)

contains(DEFINES, GUI_SUPPORT){
    QT += core gui
    QT += widgets
    HEADERS += $$PWD/eventaudiobufferfilled.h
}

DEFINES += WAVREADER_LIBRARY

include($$PWD/wavreader.pri)
include($$PWD/wavreader_sources.pri)
include($$PWD/../lib/lib.pri)
unix {
    target.path = $$PWD
    INSTALLS += target
}
