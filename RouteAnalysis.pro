#-------------------------------------------------
#
# Project created by QtCreator 2018-05-11T13:18:44
#
#-------------------------------------------------

QT       += core gui sql printsupport axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RouteAnalysis
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
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
    formroutedataquery.cpp \
    painter.cpp \
    formterminalquery.cpp \
    formnavigationquery.cpp \
    delegate.cpp \
    formberthconfig.cpp \
    mysqltablemodel.cpp \
    mysqltablemodel.cpp

HEADERS += \
        mainwindow.h \
    formroutedataquery.h \
    painter.h \
    datasource.h \
    formterminalquery.h \
    formnavigationquery.h \
    delegate.h \
    formberthconfig.h \
    mysqltablemodel.h \
    mysqltablemodel.h

FORMS += \
    formroutedataquery.ui \
    formterminalquery.ui \
    formnavigationquery.ui \
    formberthconfig.ui

RESOURCES += \
    res.qrc

RC_ICONS = web.ico
