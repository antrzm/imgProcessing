QT += gui
QT += core gui
QT += widgets

greaterThan(QT_MAJOR_VERSION, X): QT += widgets

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    sliderframe.cpp \
    tabledialog.cpp \
    imagelabel.cpp \
    imagefilter.cpp \
    imagedialog.cpp \
    hsvtransformdialog.cpp \
    histogrammaker.cpp \
    filtersdialog.cpp \
    filters.cpp \
    color_tools.cpp \
    color_converter.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mainwindow.ui \
    sliderframe.ui \
    tabledialog.ui \
    filtersdialog.ui \
    hsvtransformdialog.ui \
    imagedialog.ui

HEADERS += \
    mainwindow.h \
    color_tools.h \
    color_rgba.h \
    color_lab.h \
    color_hsv.h \
    color_converter.h \
    histogrammaker.h \
    imagedialog.h \
    tabledialog.h \
    hsvtransformdialog.h \
    filter.h \
    filtersdialog.h \
    imageeditor.h \
    imagefilter.h \
    imagelabel.h \
    sliderframe.h
