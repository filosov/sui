#-------------------------------------------------
#
# Project created by QtCreator 2012-12-13T02:55:37
#
#-------------------------------------------------

QT       -= gui

TARGET = $$qtLibraryTarget(sc-store)

TEMPLATE = lib

CONFIG += plugin

INCLUDEPATH += $$PWD/../../../core

DESTDIR = ../bin

DEFINES += SCSTORE_PLUGIN
#DEFINES += SCSTORE_LIBRARY

win32 {
    INCLUDEPATH += $$PWD/sc-machine/include
    LIBS += -L$$PWD/sc-machine/libs \
            -lsc_memory
}

OBJECTS_DIR = obj

MOC_DIR = moc

SOURCES += \
    scstoreplugin.cpp \
    scstorage.cpp \
    scstoragestream.cpp \
    util.cpp

HEADERS +=\
    scstoreplugin.hpp \
    scstorage.hpp \
    scstoragestream.hpp \
    util.hpp \
    defines.hpp

OTHER_FILES += \
    BUILD.txt
