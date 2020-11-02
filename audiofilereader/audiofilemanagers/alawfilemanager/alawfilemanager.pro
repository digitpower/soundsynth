QT       -= core gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = /usr/local/lib/soundsynth/alawfilemanager
TEMPLATE = lib
include($$PWD/alawfilemanager.pri)
include($$PWD/alawfilemanager_sources.pri)

unix {
    target.path = $$PWD
    INSTALLS += target
}
