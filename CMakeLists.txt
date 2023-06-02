cmake_minimum_required(VERSION 3.14)

project(KmrChat LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED
    COMPONENTS Core Widgets WebSockets Sql
)

set(SOURCES
    message.cpp
    user.cpp
    services/chatservice.cpp
    services/dbservice.cpp
)

set(HEADERS
    message.h
    user.h
    services/chatservice.h
    services/dbservice.h
    services/abstractdbservice.h
    services/abstractservice.h
)

add_executable(chatWebsocket
  main.cpp
  ${SOURCES} ${HEADERS}
)

target_link_libraries(chatWebsocket
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Sql
)

add_subdirectory(tests)

