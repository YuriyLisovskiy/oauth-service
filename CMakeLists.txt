cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

project(oauth2.service)

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

option(XW_USE_SQLITE3 "Link sqlite3 library and add compile XW_USE_SQLITE3 compile definition." OFF)
if (${XW_USE_SQLITE3})
    # Search for sqlite3
    find_library(
        SQLITE3 sqlite3 REQUIRED
        PATHS ${DEFAULT_INCLUDE_PATHS}
    )
    target_link_libraries(${BINARY} PUBLIC ${SQLITE3})
    add_compile_definitions(USE_SQLITE3)
endif()

# Search for xalwart.base
find_library(
    XALWART_BASE xalwart.base REQUIRED
    PATHS ${DEFAULT_INCLUDE_PATHS}
)

# Search for xalwart.server
find_library(
    XALWART_SERVER xalwart.server REQUIRED
    PATHS ${DEFAULT_INCLUDE_PATHS}
)

# Search for xalwart.crypto
find_library(
    XALWART_CRYPTO xalwart.crypto REQUIRED
    PATHS ${DEFAULT_INCLUDE_PATHS}
)

# Search for xalwart.orm
find_library(
    XALWART_ORM xalwart.orm REQUIRED
    PATHS ${DEFAULT_INCLUDE_PATHS}
)

# Search for xalwart
find_library(
    XALWART_FRAMEWORK xalwart REQUIRED
    PATHS ${DEFAULT_INCLUDE_PATHS}
)

target_link_libraries(
    ${BINARY} PUBLIC
        ${OPENSSL_LIBRARIES}
        ${XALWART_BASE}
        ${XALWART_SERVER}
        ${XALWART_CRYPTO}
        ${XALWART_ORM}
        ${XALWART_FRAMEWORK}
)
