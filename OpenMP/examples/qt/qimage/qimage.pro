QT += core gui widgets

TARGET = qimage
TEMPLATE = app

DISTFILES += \
    image1.png

SOURCES += \
    qimage.cpp

copyfiles.commands = cp $${PWD}/image1.png $${PWD}/build-qimage-debug/image1.png
copyfiles.commands = cp $${PWD}/image1.png $${PWD}/build-qimage-release/image1.png

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
