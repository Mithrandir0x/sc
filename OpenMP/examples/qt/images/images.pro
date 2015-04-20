QT += core gui widgets

TARGET = images
TEMPLATE = app

DISTFILES += \
    image1.png \
    image2.png \
    image3.png

HEADERS += \
    images.h

SOURCES += \
    images.cpp \
    main.cpp

copyfiles.commands += cp $${PWD}/image1.png $${PWD}/../build-images-release/image1.png $$escape_expand(\n\t)
copyfiles.commands += cp $${PWD}/image2.png $${PWD}/../build-images-release/image2.png $$escape_expand(\n\t)
copyfiles.commands += cp $${PWD}/image3.png $${PWD}/../build-images-release/image3.png $$escape_expand(\n\t)

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
