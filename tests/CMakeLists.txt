

set(SOURCES
    test_message.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Test)

foreach(test ${SOURCES})

    # get the limiter position
    string(FIND "${test}" "." limiter)
    string(SUBSTRING "${test}" 0 ${limiter} output)

    message("Configure TestCase ${test}")

    qt_add_executable(${output} ${test})

    target_link_libraries(${output} PRIVATE Qt6::Test)

endforeach()
