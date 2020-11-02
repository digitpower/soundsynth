HEADERS	+= $$PWD/alawfilemanager.h \
           $$PWD/../filestructs.h
INCLUDEPATH += $$PWD $$PWD/../../../lib $$PWD/../audiofilemanager


include($$PWD/../../../resampler/resampler.pri)
LIBS+=-L/usr/lib -lsamplerate
LIBS += -L$$PWD/../../../resampler -lresampler
