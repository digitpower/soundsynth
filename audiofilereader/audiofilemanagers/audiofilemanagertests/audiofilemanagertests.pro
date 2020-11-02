QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app



include($$PWD/../audiofilemanager/audiofilemanager.pri)
include($$PWD/../audiofilemanager/audiofilemanager_sources.pri)


#HEADERS += unique($$HEADERS)
#SOURCES += unique($$SOURCES)
SOURCES +=  tst_audiofilemanager.cpp


#message($$SOURCES)
