#-------------------------------------------------
#
# Project created by QtCreator 2017-11-09T08:49:24
#
#-------------------------------------------------

QT       += core gui
LIBS += -lpthread libwsock32 libws2_32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PC3
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
    logindialog.cpp \
    client.cpp \
    resultdialog.cpp \
    confirmdialog.cpp \
    thread.cpp \
    sub_his_dialog.cpp \
    runiddialog.cpp \
    des.cpp \
    errordialog.cpp

HEADERS += \
        mainwindow.h \
    logindialog.h \
    client.h \
    resultdialog.h \
    confirmdialog.h \
    thread.h \
    sub_his_dialog.h \
    runiddialog.h \
    des.h \
    errordialog.h

FORMS += \
        mainwindow.ui \
    logindialog.ui \
    resultdialog.ui \
    confirmdialog.ui \
    sub_his_dialog.ui \
    runiddialog.ui \
    errordialog.ui

RESOURCES += \
    images.qrc
