HEADERS += $$PWD/audiofilemanager_global.h \
    $$PWD/audiofilemanager.h \
    $$PWD/audiofilemanagerproxy.h

INCLUDEPATH += $$PWD $$PWD/../../../lib


include($$PWD/../pcmfilemanager/pcmfilemanager.pri)
include($$PWD/../pcmfilemanager/pcmfilemanager_sources.pri)
LIBS += -L$$PWD/../pcmfilemanager -lpcmfilemanager

include($$PWD/../alawfilemanager/alawfilemanager.pri)
include($$PWD/../alawfilemanager/alawfilemanager_sources.pri)
LIBS += -L$$PWD/../alawfilemanager -lalawfilemanager

