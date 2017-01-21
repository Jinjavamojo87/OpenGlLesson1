cmake_minimum_required(VERSION 3.6)
project(arfly)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fPIC")
set(CMAKE_AUTOMOC ON)

set(ARTOOLKIT5_ROOT $ENV{ARTOOLKIT5_ROOT})
set(OSG_ROOT $ENV{OSG_ROOT})
set(QT_ROOT $ENV{QT_ROOT})
set(COMMON_LIB_PATH "/usr/lib/x86_64-linux-gnu")

set(ARTOOLKIT_INCLUDE_PATH "${ARTOOLKIT5_ROOT}/include")
set(ARTOOLKIT_LIB_PATH "${ARTOOLKIT5_ROOT}/lib")
set(OSG_INCLUDE_PATH "${OSG_ROOT}/include")
set(OSG_LIB_PATH "${OSG_ROOT}/lib")
set(QT_INCLUDE_PATH "${QT_ROOT}/include")
set(QT_LIB_PATH "${QT_ROOT}/lib")

set(DCMAKE_BUILD_TYPE "Debug")
set(CMAKE_PREFIX_PATH "${QT_ROOT}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

set(HW_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(HW_GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gui)
include_directories(${HW_HEADER_DIR})
message("${ARTOOLKIT5_ROOT}")

include_directories(

        ${ARTOOLKIT_INCLUDE_PATH}
        ${OSG_INCLUDE_PATH}
        ${COMMON_LIB_PATH}
        ${PROJECT_SOURCE_DIR}
        ${QT_INCLUDE_PATH}
        ${QT_INCLUDE_PATH}/QtOpenGL
        ${QT_INCLUDE_PATH}/QtGui

        ${QT_INCLUDE_PATH}/Qt3DCore
        ${QT_INCLUDE_PATH}/Qt3DExtras
        ${QT_INCLUDE_PATH}/Qt3DInput
        ${QT_INCLUDE_PATH}/Qt3DLogic
        ${QT_INCLUDE_PATH}/Qt3DQuick
        ${QT_INCLUDE_PATH}/Qt3DQuickExtras
        ${QT_INCLUDE_PATH}/Qt3DQuickInput
        ${QT_INCLUDE_PATH}/Qt3DQuickRender
        ${QT_INCLUDE_PATH}/Qt3DRender
        ${QT_INCLUDE_PATH}/QtBluetooth
        ${QT_INCLUDE_PATH}/QtCLucene
        ${QT_INCLUDE_PATH}/QtConcurrent
        ${QT_INCLUDE_PATH}/QtCore
        ${QT_INCLUDE_PATH}/QtDBus
        ${QT_INCLUDE_PATH}/QtDesigner
        ${QT_INCLUDE_PATH}/QtDesignerComponents
        ${QT_INCLUDE_PATH}/QtGui
        ${QT_INCLUDE_PATH}/QtHelp
        ${QT_INCLUDE_PATH}/QtLocation
        ${QT_INCLUDE_PATH}/QtWidgets
        ${QT_INCLUDE_PATH}/QColor
        ${QT_INCLUDE_PATH}/QGLWidget
        ${QT_INCLUDE_PATH}/QDir
        ${QT_INCLUDE_PATH}/QObject
        #${QT_INCLUDE_PATH}/QString
        )

set(SOURCE_FILES
        Artoolkit/trackingSub.cpp
        Artoolkit/trackingSub.h
        Artoolkit/VirtualEnvironment.cpp
        Artoolkit/VirtualEnvironment.h
        Artoolkit/ARMarkerNFT.cpp
        Artoolkit/ARMarkerNFT.h
        Resources/qrc_Picture.cpp
        WidgetOpenGL/myOpenGL.cpp
        WidgetOpenGL/myOpenGL.h
        Viewer/OsgViewerFromWindow.h
        Viewer/OsgViewerFromWindow.cpp
        Singleton.h
        main.cpp
        MainWindow.h
        MainWindow.cpp

        MainWindow.ui
        ${ARTOOLKIT5_ROOT}/lib/SRC/AR2/coord.c
        ${ARTOOLKIT5_ROOT}/lib/SRC/AR2/jpeg.c
        ${ARTOOLKIT5_ROOT}/lib/SRC/AR2/selectTemplate.c
        ${ARTOOLKIT5_ROOT}/lib/SRC/AR2/template.c
        ${ARTOOLKIT5_ROOT}/lib/SRC/AR2/tracking.c
        ${ARTOOLKIT5_ROOT}/lib/SRC/Gl/gsub_lite.c
        MainEventHandler.cpp MainEventHandler.h
        ${OSG_ROOT}/src/osgGA/GUIEventHandler.cpp
        ${OSG_ROOT}/src/osgGA/GUIEventAdapter.cpp
        ${OSG_ROOT}/src/osgGA/EventQueue.cpp
        ${OSG_ROOT}/src/osg/Drawable.cpp
        ${OSG_ROOT}/src/osg/Object.cpp
        ${OSG_ROOT}/src/osg/Geode.cpp
        #${OSG_ROOT}/src/osgPlugins/ive/Object.cpp
        #${OSG_ROOT}/src/osgUtil/LineSegmentIntersector.cpp
        OsgIncluder.h)

#add_precompiled_header(target OsgIncluder.h FORCEINCLUDE)


set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

file(GLOB OSG_SOURCES {$OSG_ROOT}/src/osg/*.cpp {$OSG_ROOT}/src/osg/*.h)

add_executable(arfly ${SOURCE_FILES} ${OSG_SOURCES})


target_link_libraries(arfly
        ${ARTOOLKIT_LIB_PATH}/libKPM.a
        ${ARTOOLKIT_LIB_PATH}/libAR2.a
        ${ARTOOLKIT_LIB_PATH}/libARvideo.a
        ${ARTOOLKIT_LIB_PATH}/libARgsub_lite.a
        ${ARTOOLKIT_LIB_PATH}/libARgsub.a
        ${ARTOOLKIT_LIB_PATH}/libAR.a
        ${ARTOOLKIT_LIB_PATH}/libARICP.a
        ${ARTOOLKIT_LIB_PATH}/libARUtil.a
        ${ARTOOLKIT_LIB_PATH}/libARosg.a
        ${ARTOOLKIT_LIB_PATH}/libEden.a
        ${ARTOOLKIT_LIB_PATH}/libARWrapper.so


        ${COMMON_LIB_PATH}/libglut.so
        ${COMMON_LIB_PATH}/libGLU.so
        ${COMMON_LIB_PATH}/libGL.so
        ${COMMON_LIB_PATH}/libX11.so
        ${COMMON_LIB_PATH}/libm.so
        ${COMMON_LIB_PATH}/libpthread.so
        ${COMMON_LIB_PATH}/libjpeg.so
        ${COMMON_LIB_PATH}/libgstreamer-0.10.so
        ${COMMON_LIB_PATH}/libgobject-2.0.so
        ${COMMON_LIB_PATH}/libgmodule-2.0.so
        ${COMMON_LIB_PATH}/libgthread-2.0.so
        ${COMMON_LIB_PATH}/libglib-2.0.so
        ${COMMON_LIB_PATH}/libxml2.so


        ${OSG_LIB_PATH}/libosg.so
        ${OSG_LIB_PATH}/libosgDB.so
        ${OSG_LIB_PATH}/libOpenThreads.so
        ${OSG_LIB_PATH}/libosgFX.so
        ${OSG_LIB_PATH}/libosgGA.so
        ${OSG_LIB_PATH}/libosgViewer.so
        ${OSG_LIB_PATH}/libosgUtil.so
        ${OSG_LIB_PATH}/libosgText.so

        ${QT_LIB_PATH}/libQt5Core.so
        ${QT_LIB_PATH}/libQt5Gui.so
        ${QT_LIB_PATH}/libQt5OpenGL.so
        ${QT_LIB_PATH}/libQt5Widgets.so

        )

#set (ui_generate_tool "uic")
#set (ui_file "MainWindow.ui")
#execute_process(COMMAND ${ui_generate_tool} ${PROJECT_SOURCE_DIR}/${ui_file} -o ${PROJECT_SOURCE_DIR}/ui_MainWindow.h)

#set(resource_generate_tool "rcc")
#set (resource_file "Picture.qrc")
#execute_process(COMMAND ${resource_generate_tool} ${PROJECT_SOURCE_DIR}/${resource_file} -o ${PROJECT_SOURCE_DIR}/Resources/qrc_Picture.cpp)

