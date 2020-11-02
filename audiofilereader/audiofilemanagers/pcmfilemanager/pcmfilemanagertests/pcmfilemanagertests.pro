QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testpcmfilemanager.cpp

include($$PWD/../pcmfilemanager.pri)
include($$PWD/../pcmfilemanager_sources.pri)

