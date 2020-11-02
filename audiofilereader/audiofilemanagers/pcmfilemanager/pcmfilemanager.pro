QT       -= core gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = pcmfilemanager
TEMPLATE = lib
include($$PWD/pcmfilemanager.pri)
include($$PWD/pcmfilemanager_sources.pri)

unix {
    target.path = $$PWD
    INSTALLS += target
}
