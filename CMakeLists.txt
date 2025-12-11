cmake_minimum_required(VERSION 3.16)
project(MouseTrajectoryDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用 Qt 的自动处理宏与资源
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 COMPONENTS Widgets REQUIRED)

add_executable(MouseTrajectoryDemo
    main.cpp
    trajectorywidget.cpp
    trajectorywidget.h
)

target_link_libraries(MouseTrajectoryDemo PRIVATE Qt6::Widgets)
