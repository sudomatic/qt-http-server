
QT += core network

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/QtHttpClientWrapper.cpp \
    $$PWD/QtHttpHeader.cpp \
    $$PWD/QtHttpReply.cpp \
    $$PWD/QtHttpRequest.cpp \
    $$PWD/QtHttpServer.cpp

HEADERS += \
    $$PWD/QtHttpClientWrapper.h \
    $$PWD/QtHttpHeader.h \
    $$PWD/QtHttpReply.h \
    $$PWD/QtHttpRequest.h \
    $$PWD/QtHttpServer.h

DISTFILES += \
    $$PWD/LICENSE.md
