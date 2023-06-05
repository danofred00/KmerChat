
# set core sources files
set(SOURCES
    message.cpp
    user.cpp
    request.cpp
    response.cpp
    abstractnetworkmessage.cpp
    services/abstractdbservice.cpp
    models/usermodel.cpp
)

# set core headers files
set(HEADERS
    message.h
    user.h
    request.h
    response.h
    abstractnetworkmessage.h
    services/abstractdbservice.h
    services/abstractservice.h
    models/usermodel.h
)

add_library(${CORE_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${CORE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Core PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
)
