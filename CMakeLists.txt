cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)

project(oauth-service)

set(BINARY application)

set(CMAKE_CXX_FLAGS "-pthread")

set(
    DEFAULT_INCLUDE_PATHS
        "/usr/local"
        "/usr"
)

foreach(ENTRY ${DEFAULT_INCLUDE_PATHS})
    include_directories(${ENTRY}/include)
    link_directories(${ENTRY}/lib)
endforeach()

# Search for OpenSSL
find_package(OpenSSL 1.1 REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

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

if (NOT APPLE)
    target_link_libraries(${BINARY} PUBLIC stdc++fs)
endif()

option(XW_USE_SQLITE3 "Search for SQLite3." OFF)
if (${XW_USE_SQLITE3})
    find_package(SQLite3 3 REQUIRED)
    include_directories(${SQLite3_INCLUDE_DIRS})
    target_link_libraries(${BINARY} PUBLIC ${SQLite3_LIBRARIES})
    add_compile_definitions(USE_SQLITE3)
endif()

option(XW_USE_POSTGRESQL "Search for PostgreSQL." OFF)
if (${XW_USE_POSTGRESQL})
    find_package(PostgreSQL 12 REQUIRED)
    target_link_libraries(${BINARY} PUBLIC ${PostgreSQL_LIBRARIES})
    include_directories(${PostgreSQL_INCLUDE_DIRS})
    add_compile_definitions(USE_POSTGRESQL)
endif()

set(XALWART_LIBRARIES)
function(FIND_XALWART_LIBRARY PART)
    find_library(
        XALWART_${PART} xalwart${PART} REQUIRED
        PATHS ${DEFAULT_INCLUDE_PATHS}
    )
    set(XALWART_LIBRARIES ${XALWART_LIBRARIES} ${XALWART_${PART}} PARENT_SCOPE)
endfunction(FIND_XALWART_LIBRARY)

find_xalwart_library(.base)
find_xalwart_library(.server)
find_xalwart_library(.crypto)
find_xalwart_library(.orm)
find_xalwart_library("")

target_link_libraries(${BINARY} PUBLIC ${OPENSSL_LIBRARIES} ${XALWART_LIBRARIES})

option(CONFIGURE_TESTS "Configure tests." OFF)
if (${CONFIGURE_TESTS})
    add_subdirectory(tests)
endif()
