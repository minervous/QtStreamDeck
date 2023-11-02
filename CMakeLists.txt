cmake_minimum_required(VERSION 3.14)

project(StreamDeckLib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Usb Qml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Usb Qml)
qt_policy(SET QTP0001 NEW)

add_subdirectory(src)
add_subdirectory(plugin)
add_subdirectory(examples)
