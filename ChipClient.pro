#-------------------------------------------------
#
# Project created by QtCreator 2023-12-21T09:56:48
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChipClient
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

CONFIG += c++11

SOURCES += \
    StilParserTool.cpp \
    TestVector.cpp \
    VectorEditTool.cpp \
    WaveformView.cpp \
        main.cpp \
        MainWindow.cpp \
    Def.cpp \
    DevConfigwidget.cpp \
    Monitorwidget.cpp \
    Msglogwidget.cpp \
    Progressctrlwidget.cpp \
    Sds6034widget.cpp \
    Teststepinfowidget.cpp \
    Udp6730widget.cpp \
    Utg4082widget.cpp \
    Utl8512widget.cpp

HEADERS += \
        MainWindow.h \
    Def.h \
    DevConfigwidget.h \
    Monitorwidget.h \
    Msglogwidget.h \
    Progressctrlwidget.h \
    Sds6034widget.h \
    Singleton.h \
    StilParserTool.h \
    TestVector.h \
    Teststepinfowidget.h \
    Udp6730widget.h \
    Utg4082widget.h \
    Utl8512widget.h \
    VectorEditTool.h \
    WaveformView.h

FORMS += \
        MainWindow.ui \
        StilParserTool.ui \
        WaveformView.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/Lib/ -lSds6000Lib -lUdp6730Lib -lUtg4000Lib -lUtl8500Lib -lvisa32 -llibstil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/Lib/ -lSds6000Lib -lUdp6730Lib -lUtg4000Lib -lUtl8500Lib -lvisa32 -llibstild

INCLUDEPATH += $$PWD/Include
DEPENDPATH += $$PWD/Include

