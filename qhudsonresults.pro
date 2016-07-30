TEMPLATE = app
TARGET = qhudsonresults
VERSION = 1.0.0
QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4) {
           QT += widgets
}
#QMAKE_CXXFLAGS = -Wno-unused-parameter
#CONFIG += console debug
DEPENDPATH += . src
INCLUDEPATH += . src

HEADERS += src/HtmlItemDelegate.h \
           src/MainWindow.h \
           src/TestItem.h

FORMS += src/MainWindow.ui

SOURCES += src/main.cpp \
           src/HtmlItemDelegate.cpp \
           src/MainWindow.cpp \
           src/TestItem.cpp

RESOURCES += src/resources.qrc
RC_FILE += src/QHudsonResults.rc
