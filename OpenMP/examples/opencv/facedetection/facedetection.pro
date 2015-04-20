TARGET = facedetection
TEMPLATE = app

# OpenCV Dependencies
#
# Set the path to the OpenCV compiled distribution.
#
# Normally, for a *nix environment, it would be found at: /usr/local/include/opencv
#
# But for windows, it is required to compile it. Please refer to the next link for
# more information about compiling OpenCV in Windows:
#
#     https://zahidhasan.wordpress.com/2014/08/19/qt-5-3-1-64-bit-mingw-windows/
#
OPENCV_PATH        = D:\\opencv-mingw\\install
LIB_OPENCV_VERSION = 2411
#
# PLEASE, DO NOT TOUCH THESE VARIABLES
INCLUDEPATH += $${OPENCV_PATH}\\include
INCLUDEPATH += $${OPENCV_PATH}\\include\\opencv
INCLUDEPATH += $${OPENCV_PATH}\\include\\opencv2
LIBS += -L"$${OPENCV_PATH}\\x64\\mingw\\lib"
LIBS += -lopencv_core$${LIB_OPENCV_VERSION}
LIBS += -lopencv_highgui$${LIB_OPENCV_VERSION}
LIBS += -lopencv_imgproc$${LIB_OPENCV_VERSION}
#

DISTFILES += \
    haarcascade_frontalface_alt.xml \
    image1.jpg \
    image2.jpg

SOURCES += \
    face.cpp
