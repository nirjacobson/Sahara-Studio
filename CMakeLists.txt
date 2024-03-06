cmake_minimum_required(VERSION 3.5)

project(Sahara-Studio VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Xml Widgets OpenGL OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Xml Widgets OpenGL OpenGLWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

include_directories("../QCollada")
include_directories("../Sahara")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Sahara-Studio
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        colorwidget/colorwidget.cpp
        colorwidget/colorwidget.h
        icons/add.png
        icons/axes.png
        icons/camera.png
        icons/counter.png
        icons/cursor.png
        icons/eye.png
        icons/grid.png
        icons/light.png
        icons/lock.png
        icons/mesh.png
        icons/model.png
        icons/move.png
        icons/plane.png
        icons/scene.png
        icons/surface.png
        icons/unlock.png
        linkwidget/linkwidget.cpp
        linkwidget/linkwidget.h
        nodedetailwidget/nodedetailwidget.cpp
        nodedetailwidget/nodedetailwidget.h
        nodedetailwidget/nodedetailwidget.ui
        nodeitemdetailwidget/camerawidget/camerawidget.cpp
        nodeitemdetailwidget/camerawidget/camerawidget.h
        nodeitemdetailwidget/camerawidget/camerawidget.ui
        nodeitemdetailwidget/modelwidget/geometryitemmodel.cpp
        nodeitemdetailwidget/modelwidget/geometryitemmodel.h
        nodeitemdetailwidget/modelwidget/geometrymodel.cpp
        nodeitemdetailwidget/modelwidget/geometrymodel.h
        nodeitemdetailwidget/modelwidget/materialwidget.cpp
        nodeitemdetailwidget/modelwidget/materialwidget.h
        nodeitemdetailwidget/modelwidget/materialwidget.ui
        nodeitemdetailwidget/modelwidget/modelwidget.cpp
        nodeitemdetailwidget/modelwidget/modelwidget.h
        nodeitemdetailwidget/modelwidget/modelwidget.ui
        nodeitemdetailwidget/pointlightwidget/pointlightwidget.cpp
        nodeitemdetailwidget/pointlightwidget/pointlightwidget.h
        nodeitemdetailwidget/pointlightwidget/pointlightwidget.ui
        nodeitemdetailwidget/scenewidget/scenewidget.cpp
        nodeitemdetailwidget/scenewidget/scenewidget.h
        nodeitemdetailwidget/scenewidget/scenewidget.ui
        scenegraphwidget/scenegraphitemmodel.cpp
        scenegraphwidget/scenegraphitemmodel.h
        scenegraphwidget/scenegraphtreeview.cpp
        scenegraphwidget/scenegraphtreeview.h
        scenegraphwidget/scenegraphwidget.cpp
        scenegraphwidget/scenegraphwidget.h
        scenegraphwidget/scenegraphwidget.ui
        toolswidget/tool/move.cpp
        toolswidget/tool/move.h
        toolswidget/tool/select.cpp
        toolswidget/tool/select.h
        toolswidget/tool/tool.cpp
        toolswidget/tool/tool.h
        toolswidget/toolswidget.cpp
        toolswidget/toolswidget.h
        toolswidget/toolswidget.ui
        CMakeLists.txt
        icons.qrc
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Sahara-Studio APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Sahara-Studio SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Sahara-Studio
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Sahara-Studio PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                            Qt${QT_VERSION_MAJOR}::Xml
                                            Qt${QT_VERSION_MAJOR}::OpenGL
                                            Qt${QT_VERSION_MAJOR}::OpenGLWidgets
                                            ${CMAKE_CURRENT_SOURCE_DIR}/../QCollada/build/Desktop-Debug/libQCollada.so
                                            ${CMAKE_CURRENT_SOURCE_DIR}/../Sahara/build/Desktop-Debug/libSahara.so)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Sahara-Studio)
endif()
set_target_properties(Sahara-Studio PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Sahara-Studio
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Sahara-Studio)
endif()
