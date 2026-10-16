QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# network 模块：QTcpSocket / QTcpServer

SOURCES += \
    main.cpp \
    widget.cpp \
    chat.cpp

HEADERS += \
    widget.h \
    chat.h

FORMS += \
    widget.ui \
    chat.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
