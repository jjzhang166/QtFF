#-------------------------------------------------
#
# Project created by QtCreator 2017-07-25T09:21:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ffplayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    myffmpeg.cpp \
    videoplayer.cpp \
    test.cpp

HEADERS += \
        mainwindow.h \
    myffmpeg.h \
    videoplayer.h \
    test.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += $$PWD/ff33/include
LIBS += $$PWD/ff33/lib/avcodec.lib\
        $$PWD/ff33/lib/avdevice.lib\
        $$PWD/ff33/lib/avfilter.lib\
        $$PWD/ff33/lib/avformat.lib\
        $$PWD/ff33/lib/avutil.lib\
        $$PWD/ff33/lib/postproc.lib\
        $$PWD/ff33/lib/swresample.lib\
        $$PWD/ff33/lib/swscale.lib\


