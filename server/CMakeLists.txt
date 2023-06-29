
# set sources files for the server
set(SOURCES
    configloader.cpp
    appserver.cpp
    services/chatservice.cpp
    services/dbservice.cpp
    services/authservice.cpp
    services/appservice.cpp
    models/connectionmodel.cpp
)

# set headers files for the server
set(HEADERS
    configloader.h
    appserver.h
    services/chatservice.h
    services/dbservice.h
    services/authservice.h
    services/appservice.h
    models/connectionmodel.h
)

add_executable(${SERVER_NAME}
    config.json
    main.cpp
    ${SOURCES} ${HEADERS})

target_link_libraries(${SERVER_NAME} PRIVATE ${CORE_NAME})

target_include_directories(${SERVER_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(${SERVER_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::WebSockets
)

# copy some files
file(COPY config.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
