HEADERS += $$PWD/alawwaveformgenerator_global.h \
    $$PWD/alawwaveformgenerator.h
INCLUDEPATH += $$PWD

include($$PWD/../audiofilereader/audiofilemanagers/alawdecoder/alawdecoder.pri)
LIBS += -L/usr/local/lib/soundsynth -lalawdecoder
