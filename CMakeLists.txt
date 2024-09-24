cmake_minimum_required(VERSION 3.16)

project(mainwindow2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets TextToSpeech)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets TextToSpeech)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mainwindow2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(mainwindow2 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(mainwindow2
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(mainwindow2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::TextToSpeech)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_finalize_executable(mainwindow2)
endif()

# 设置目标属性
set_target_properties(mainwindow2 PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS mainwindow2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
