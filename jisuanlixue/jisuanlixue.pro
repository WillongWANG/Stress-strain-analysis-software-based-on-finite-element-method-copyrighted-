QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    coordinateitem.cpp \
    customarrowitem.cpp \
    customellipseitem.cpp \
    customrectitem.cpp \
    customrectitem_1.cpp \
    hinge.cpp \
    main.cpp \
    mainwindow.cpp \
    mygraphicsscene.cpp

HEADERS += \
    coordinateitem.h \
    customarrowitem.h \
    customellipseitem.h \
    customrectitem.h \
    customrectitem_1.h \
    hinge.h \
    mainwindow.h \
    mygraphicsscene.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32: LIBS += -LF:/soft/qt/gmsh-4.11.1-Windows64-sdk/gmsh-4.11.1-Windows64-sdk/lib/ -lgmsh

INCLUDEPATH += F:/soft/qt/gmsh-4.11.1-Windows64-sdk/gmsh-4.11.1-Windows64-sdk/include
DEPENDPATH += F:/soft/qt/gmsh-4.11.1-Windows64-sdk/gmsh-4.11.1-Windows64-sdk/include

INCLUDEPATH += $$PWD Eigen

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
LIBS += -lgomp
