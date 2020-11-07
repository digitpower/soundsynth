include($$PWD/../soundsynth.pri)
contains(DEFINES, GUI_SUPPORT) {
    QT += widgets
    HEADERS += $$PWD/mediaplayer.h
    SOURCES += $$PWD/mediaplayer.cpp
    FORMS += $$PWD/mediaplayer.ui
}
