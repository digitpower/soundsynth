#-------------------------------------------------
#
# Project created by QtCreator 2020-07-11T10:51:30
#
#-------------------------------------------------

QT       -= core gui

QMAKE_CXXFLAGS += -std=c++11
TARGET = ugen_loudness
TEMPLATE = lib

DEFINES += UGEN_LOUDNESS_LIBRARY

include($$PWD/ugen_loudness.pri)
include($$PWD/ugen_loudness_sources.pri)
include($$PWD/../lib/lib.pri)
unix {
    target.path = $$PWD
    INSTALLS += target
}
