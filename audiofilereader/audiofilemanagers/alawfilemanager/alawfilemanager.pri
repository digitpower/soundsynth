HEADERS	+= $$PWD/alawfilemanager.h \
           $$PWD/../filestructs.h \
           $$PWD/../audiofilemanager/audiofilemanager.h
INCLUDEPATH += $$PWD $$PWD/../../../lib


include($$PWD/../../../resampler/resampler.pri)
include($$PWD/../alawdecoder/alawdecoder.pri)
LIBS += -L/usr/lib -lsamplerate
LIBS += -L/usr/local/lib/soundsynth -lresampler -lalawdecoder
