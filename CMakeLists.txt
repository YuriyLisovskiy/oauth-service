cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

project(oauth2.service)
set(BINARY application)

if (NOT APPLE)
    set(CMAKE_CXX_FLAGS "-pthread")
endif()

set(ROOT_DIR /usr/local)
set(INCLUDE_DIR ${ROOT_DIR}/include)
set(LIB_DIR ${ROOT_DIR}/lib)

include_directories(${INCLUDE_DIR})
link_directories(${LIB_DIR})

if (APPLE)
    set(XW_OPENSSL_DIR "openssl@1.1/1.1.1l" CACHE STRING "OpenSSL version to use for compiling lib.")

    set(XW_OPENSSL_OPT_HOMEBREW_INCLUDE /opt/homebrew/Cellar/${XW_OPENSSL_DIR}/include)
    if (EXISTS ${XW_OPENSSL_OPT_HOMEBREW_INCLUDE})
        include_directories(${XW_OPENSSL_OPT_HOMEBREW_INCLUDE})
    endif()

    set(XW_OPENSSL_OPT_HOMEBREW_LIB /opt/homebrew/Cellar/${XW_OPENSSL_DIR}/lib)
    if (EXISTS ${XW_OPENSSL_OPT_HOMEBREW_LIB})
        link_directories(${XW_OPENSSL_OPT_HOMEBREW_LIB})
    endif()

    set(XW_OPENSSL_USR_LOCAL_INCLUDE /usr/local/Cellar/${XW_OPENSSL_DIR}/include)
    if (EXISTS ${XW_OPENSSL_USR_LOCAL_INCLUDE})
        include_directories(${XW_OPENSSL_USR_LOCAL_INCLUDE})
    endif()

    set(XW_OPENSSL_USR_LOCAL_LIB /usr/local/Cellar/${XW_OPENSSL_DIR}/lib)
    if (EXISTS ${XW_OPENSSL_USR_LOCAL_LIB})
        link_directories(${XW_OPENSSL_USR_LOCAL_LIB})
    endif()
endif()


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

if (NOT APPLE)
    target_link_libraries(${BINARY} PUBLIC stdc++fs)
endif()

target_link_libraries(
    ${BINARY} PUBLIC
        ${XALWART_FRAMEWORK}.base
        ${XALWART_FRAMEWORK}.server
        ${XALWART_FRAMEWORK}.crypto
        ${XALWART_FRAMEWORK}.orm
        ${XALWART_FRAMEWORK}
)
