set(CMAKE_CXX_FLAGS "-pthread")

set(BINARY unittests)

function(add_tests SUB_DIR SUB_NAME)
    set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests/${SUB_DIR})
    file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${TESTS_DIR}*.cpp
    )
    list(
        FILTER TEST_SOURCES
        EXCLUDE REGEX "^.*/(include|lib$|media|static|templates|cmake-build-*)/?.*"
    )
    foreach(entry ${TEST_SOURCES})
        if (IS_DIRECTORY ${entry})
            list(REMOVE_ITEM TEST_SOURCES ${entry})
        endif()
    endforeach()
    list(FILTER TEST_SOURCES EXCLUDE REGEX ".*main.cpp$")

    set(FULL_BIN ${BINARY}-${SUB_NAME})
    add_executable(${FULL_BIN} ${TEST_SOURCES} ${TESTS_DIR}/main.cpp)
    if (NOT APPLE)
        target_link_libraries(${FULL_BIN} PUBLIC stdc++fs)
    endif()
    if (WIN32)
        target_link_libraries(${FULL_BIN} PUBLIC gtest_main)
    else()
        target_link_libraries(${FULL_BIN} PUBLIC gtest)
    endif()
    target_link_libraries(${FULL_BIN} PUBLIC ${OPENSSL_LIBRARIES} ${XALWART_LIBRARIES})
endfunction()

function(add_sub_tests SUB_NAME)
    add_tests(${SUB_NAME}/ ${SUB_NAME})
endfunction(add_sub_tests)

add_tests("" all)
add_sub_tests(users)
