HEADERS += $$PWD/audiofilemanager_global.h \
    $$PWD/audiofilemanager.h \
    $$PWD/audiofilemanagerproxy.h

INCLUDEPATH += $$PWD $$PWD/../../../lib


include($$PWD/../pcmfilemanager/pcmfilemanager.pri)
LIBS += -L/usr/local/lib/soundsynth -lpcmfilemanager

include($$PWD/../alawfilemanager/alawfilemanager.pri)
LIBS += -L/usr/local/lib/soundsynth -lalawfilemanager

