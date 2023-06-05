cmake_minimum_required(VERSION 3.14)

project(KmrChat LANGUAGES CXX)

#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED
    COMPONENTS Core Widgets WebSockets Sql
)

# some default config
set(CORE_NAME Core)
set(SERVER_NAME Server)
set(CLIENT_NAME Client)
#
add_subdirectory(core)

add_subdirectory(client)

add_subdirectory(server)

add_subdirectory(tests)

