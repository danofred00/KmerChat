

# set sources files for the server
set(SOURCES
    test.cpp
)

# set headers files for the server
set(HEADERS
    test.h
)

add_executable(${CLIENT_NAME}
    main.cpp
    ${SOURCES} ${HEADERS})

target_link_libraries(${CLIENT_NAME} PRIVATE ${CORE_NAME})

target_include_directories(${CLIENT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(${CLIENT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Sql
)
