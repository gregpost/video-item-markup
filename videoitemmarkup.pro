TEMPLATE = app
TARGET = videographicsitem

QT += multimedia multimediawidgets

HEADERS   += \
    document.h \
    mainwindow.h \
    mygraphicsview.h

SOURCES   += main.cpp \
             document.cpp \
             mainwindow.cpp \
             mygraphicsview.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/videographicsitem
INSTALLS += target

QT+=widgets

FORMS += \
    mainwindow.ui

RESOURCES += \
    res.qrc

DISTFILES +=
