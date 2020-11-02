QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testalawfilemanager.cpp

include($$PWD/../alawfilemanager.pri)
include($$PWD/../alawfilemanager_sources.pri)

