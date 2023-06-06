cmake_minimum_required(VERSION 3.14)

project(KmrChatClient LANGUAGES CXX)

set(CMAKE_AUTOMOC On)
set(CMAKE_AUTORCC On)
set(CMAKE_AUTOUIC On)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql WebSockets)

# set sources files for the server
set(SOURCES
    serversocket.cpp
    services/authservice.cpp
)

# set headers files for the server
set(HEADERS
    serversocket.h
    services/authservice.h
)

add_executable(${CLIENT_NAME}
    main.cpp
    ui/resources.qrc
    ${SOURCES} ${HEADERS})

set_target_properties(${CLIENT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER cm.danofred.kmerchat
    #MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${CLIENT_NAME} PRIVATE ${CORE_NAME})

target_include_directories(${CLIENT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(${CLIENT_NAME} PRIVATE
    # Qt${QT_VERSION_MAJOR}::Quick
    # Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Sql
)

#if(${ANDROID})
#    message("Building For Android System")

#    install(TARGETS ${CLIENT_NAME}
#        BUNDLE DESTINATION .
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

#endif()
