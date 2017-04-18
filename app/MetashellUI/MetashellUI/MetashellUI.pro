#-------------------------------------------------
#
# Project created by QtCreator 2016-01-20T21:37:18
#
#-------------------------------------------------

QT       += core gui

CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MetashellUI
TEMPLATE = app


SOURCES += main.cpp\
        metashellmainwindow.cpp \
    metashellinterpreter.cpp \
    consolewidget.cpp

HEADERS  += metashellmainwindow.h \
    metashellinterpreter.h \
    consolewidget.h

FORMS    += metashellmainwindow.ui
