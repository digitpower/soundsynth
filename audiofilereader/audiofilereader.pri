DEFINES += GENERAL_AUDIOFILE_CODE
HEADERS += $$PWD/audiofilereader.h \
           $$PWD/../ugen.h
INCLUDEPATH += $$PWD $$PWD/..
contains(DEFINES, GENERAL_AUDIOFILE_CODE) {
    include($$PWD/audiofilemanagers/audiofilemanager/audiofilemanager.pri)
    include($$PWD/audiofilemanagers/audiofilemanager/audiofilemanager_sources.pri)
    LIBS += -L$$PWD/audiofilemanagers/audiofilemanager -laudiofilemanager
}
