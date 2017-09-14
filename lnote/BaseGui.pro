#-------------------------------------------------
#
# Project created by QtCreator 2017-09-08T07:42:57
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include($$PWD/inputchinese/inputchinese.pri)

TARGET = BaseGui
TEMPLATE = app

SOURCES += \
    main/main.cpp \
    windows/mainwidget.cpp \
    common/basesetup.cpp \
    common/alistwidget.cpp \
    windows/asciscintilla.cpp \
    common/atabwidget.cpp

HEADERS += \
    windows/mainwidget.h \
    common/basesetup.h \
    common/alistwidget.h \
    windows/asciscintilla.h \
    common/basedefine.h \
    common/atabwidget.h


MOC_DIR             = temp/moc
RCC_DIR             = temp/rcc
UI_DIR              = temp/ui
OBJECTS_DIR         = temp/obj
DESTDIR             = bin

INCLUDEPATH += /home/xiaoxie/source/linux_note/lnote/Qsci
LIBS += -L/home/xiaoxie/source/linux_note/lnote/libqsci -lqscintilla2_qt5

RESOURCES += \
    base.qrc

DISTFILES += \
    Readme




