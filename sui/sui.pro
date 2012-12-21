#-------------------------------------------------
#
# Project created by QtCreator 2011-06-04T23:28:59
#
#-------------------------------------------------

QT       += core

#TARGET = sui
TEMPLATE = subdirs

CONFIG += ordered

CONFIG (debug, debug|release) {
        DESTDIR = ../bin/debug
} else {
        DESTDIR = ../bin/release
}

SUBDIRS = core \
            plugins/sc-core \
            plugins/scg \
            app #\
#            plugins/ScStoragePluginProject





#CONFIG += ordered

#CONFIG (debug, debug|release) {
#        OBJECTS_DIR = build/debug
#} else {
#        OBJECTS_DIR = build/release
#}

#MOC_DIR = build/moc

