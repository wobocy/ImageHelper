cmake_minimum_required(VERSION 3.16)
project(ImageProcessor VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

qt6_standard_project_setup()

set(SOURCES
    main.cpp
    mainwindow.cpp
    imageprocessor.cpp
)

set(HEADERS
    mainwindow.h
    imageprocessor.h
)

qt6_add_executable(ImageProcessor ${SOURCES} ${HEADERS})

target_link_libraries(ImageProcessor Qt6::Core Qt6::Widgets Qt6::Gui)

# 设置Windows下的应用程序图标和属性
if(WIN32)
    set_target_properties(ImageProcessor PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()