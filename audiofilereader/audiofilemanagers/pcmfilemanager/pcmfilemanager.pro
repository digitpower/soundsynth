QT       -= core gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = /usr/local/lib/soundsynth/pcmfilemanager
TEMPLATE = lib
include($$PWD/pcmfilemanager.pri)
include($$PWD/pcmfilemanager_sources.pri)

unix {
    target.path = $$PWD
    INSTALLS += target
}
