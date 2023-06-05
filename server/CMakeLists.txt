
# set sources files for the server
set(SOURCES
    services/chatservice.cpp
    services/dbservice.cpp
    services/authservice.cpp
    services/appservice.cpp
)

# set headers files for the server
set(HEADERS
    services/chatservice.h
    services/dbservice.h
    services/authservice.h
    services/appservice.h
)

add_executable(${SERVER_NAME}
    main.cpp
    ${SOURCES} ${HEADERS})

target_link_libraries(${SERVER_NAME} PRIVATE ${CORE_NAME})

target_include_directories(${SERVER_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(${SERVER_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Sql
)
