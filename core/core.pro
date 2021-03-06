#-------------------------------------------------
#
# Project created by QtCreator 2014-02-19T13:12:41
#
#-------------------------------------------------

QT += gui widgets network

TARGET = core
TEMPLATE = lib

DEFINES += CORE_LIBRARY

SOURCES += \
    coresettings.cpp \
    coreapplication.cpp \
    proxyaction.cpp \
    connection.cpp \
    configpage.cpp \
    configwidget.cpp \
    contextmanager.cpp

HEADERS +=\
        core_global.h \
    coresettings.h \
    coreapplication.h \
    proxyaction.h \
    connection.h \
    configpage.h \
    configwidget.h \
    contextmanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logging/release/ -llogging
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logging/debug/ -llogging
else:unix: LIBS += -L$$OUT_PWD/../logging/ -llogging

INCLUDEPATH += $$PWD/../logging
DEPENDPATH += $$PWD/../logging

FORMS += \
    configwidget.ui
