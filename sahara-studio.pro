#-------------------------------------------------
#
# Project created by QtCreator 2019-12-08T14:25:42
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sahara-studio
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    nodeitemdetailwidget/scenewidget/scenewidget.cpp \
    scenegraphwidget/scenegraphwidget.cpp \
    scenegraphwidget/scenegraphitemmodel.cpp \
    scenegraphwidget/scenegraphtreeview.cpp \
    nodedetailwidget/nodedetailwidget.cpp \
    nodeitemdetailwidget/camerawidget/camerawidget.cpp \
    colorwidget/colorwidget.cpp \
    nodeitemdetailwidget/pointlightwidget/pointlightwidget.cpp \
    toolswidget/toolswidget.cpp \
    toolswidget/tool/tool.cpp \
    toolswidget/tool/move.cpp \
    toolswidget/tool/select.cpp \
    nodeitemdetailwidget/modelwidget/modelwidget.cpp \
    nodeitemdetailwidget/modelwidget/geometryitemmodel.cpp \
    nodeitemdetailwidget/modelwidget/geometrymodel.cpp \
    nodeitemdetailwidget/modelwidget/materialwidget.cpp \
    linkwidget/linkwidget.cpp

HEADERS += \
        mainwindow.h \
    nodeitemdetailwidget/scenewidget/scenewidget.h \
    scenegraphwidget/scenegraphwidget.h \
    scenegraphwidget/scenegraphitemmodel.h \
    scenegraphwidget/scenegraphtreeview.h \
    nodedetailwidget/nodedetailwidget.h \
    nodeitemdetailwidget/camerawidget/camerawidget.h \
    colorwidget/colorwidget.h \
    nodeitemdetailwidget/pointlightwidget/pointlightwidget.h \
    toolswidget/toolswidget.h \
    toolswidget/tool/tool.h \
    toolswidget/tool/move.h \
    toolswidget/tool/select.h \
    nodeitemdetailwidget/modelwidget/modelwidget.h \
    nodeitemdetailwidget/modelwidget/geometryitemmodel.h \
    nodeitemdetailwidget/modelwidget/geometrymodel.h \
    nodeitemdetailwidget/modelwidget/materialwidget.h \
    linkwidget/linkwidget.h

FORMS += \
        mainwindow.ui \
    nodeitemdetailwidget/scenewidget/scenewidget.ui \
    scenegraphwidget/scenegraphwidget.ui \
    nodedetailwidget/nodedetailwidget.ui \
    nodeitemdetailwidget/camerawidget/camerawidget.ui \
    nodeitemdetailwidget/pointlightwidget/pointlightwidget.ui \
    toolswidget/toolswidget.ui \
    nodeitemdetailwidget/modelwidget/modelwidget.ui \
    nodeitemdetailwidget/modelwidget/materialwidget.ui

INCLUDEPATH += \
    ../QCollada \
    ../Sahara

LIBS += \
    -L../build-Sahara-Desktop-Debug -lSahara \
    -L../build-QCollada-Desktop-Debug -lQCollada

QMAKE_RPATHDIR += ../build-Sahara-Desktop-Debug
QMAKE_RPATHDIR += ../build-QCollada-Desktop-Debug

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
