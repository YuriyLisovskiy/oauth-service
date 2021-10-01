cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

project(oauth2.service)
set(BINARY application)

set(CMAKE_CXX_FLAGS "-pthread")

# Load and filter project sources.
file(
    GLOB_RECURSE SOURCES
    LIST_DIRECTORIES true
    src/*.h src/*.cpp
)
list(
	FILTER SOURCES
	EXCLUDE REGEX "^.*/(include|lib$|media|static|templates|cmake-build-*)/?.*"
)
foreach(entry ${SOURCES})
    if (IS_DIRECTORY ${entry})
        list(REMOVE_ITEM SOURCES ${entry})
    endif()
endforeach()

add_executable(${BINARY} ${SOURCES})

# Setup and link framework.
set(XALWART_FRAMEWORK xalwart)

option(XW_USE_SQLITE3 "Link sqlite3 library and add compile XW_USE_SQLITE3 compile definition." OFF)
if (${XW_USE_SQLITE3})
    # Use sqlite3 driver.
    add_compile_definitions(USE_SQLITE3)
    target_link_libraries(${BINARY} PUBLIC sqlite3)
endif()

target_link_libraries(${BINARY} PUBLIC stdc++fs)
target_link_libraries(
    ${BINARY} PUBLIC
        ${XALWART_FRAMEWORK}.base
        ${XALWART_FRAMEWORK}.server
        ${XALWART_FRAMEWORK}.orm
        ${XALWART_FRAMEWORK}
)
