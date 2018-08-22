

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sever_multithread
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    serverthread.cpp \
    server.cpp \
    mysocket.cpp \
    mattoqimage.cpp

HEADERS  += dialog.h \
    serverthread.h \
    server.h \
    mysocket.h \
    mattoqimage.h

FORMS    += dialog.ui

INCLUDEPATH+=E:/QT_opencv/install/include/opencv \
E:/QT_opencv/install/include/opencv2 \
E:/QT_opencv/install/include \

LIBS+=E:/QT_opencv/install/x64/vc14/lib/*.lib
