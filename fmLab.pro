#-------------------------------------------------
#
# Project created by QtCreator 2014-06-19T19:36:58
#
#-------------------------------------------------

QMAKE_MAC_SDK = macosx10.9
QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += "/usr/local/include"
LIBS += -L/usr/local/lib -lportaudio -framework CoreServices -framework CoreAudio -framework AudioUnit -framework AudioToolBox

QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fmLab
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    GLWidget.cpp \
    vizwindow.cpp

HEADERS  += mainwindow.h \
    fm.hpp \
    audio.hpp \
    GLWidget.h \
    vizwindow.h

FORMS    += mainwindow.ui \
    vizwindow.ui
