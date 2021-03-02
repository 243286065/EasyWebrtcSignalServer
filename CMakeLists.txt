cmake_minimum_required(VERSION 3.6.0)

project(EasyWebrtcSignalServer)

message("-- system arch:  ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "current platform: ${CMAKE_SYSTEM_NAME} ")

add_definitions(-DOS_ARCH=${CMAKE_SYSTEM_NAME})

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
    message(STATUS "Compile Debug")
ELSE()
    add_definitions(-DRELEASE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pthread")
    message(STATUS "Compile Release")
ENDIF()

IF( ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_WIN32_WINNT=0x0601 /W4 /wd4068 /wd4702 /wd4996" )
ENDIF()

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_OUTPUT_DIR ${PROJECT_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR})
message("-- output path:  ${PROJECT_OUTPUT_DIR} ")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake" )

file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)

find_package(asio REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${ASIO_INCLUDE})

include_directories("src/lib/websocketpp")
include_directories("src/lib/base/src")

add_subdirectory(src/lib/base)

set(
    PROJECT_SOURCE
    src/main.cpp
    src/server/signal_server.h
    src/server/signal_server.cpp
    src/protocol/message.h
    src/protocol/message.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})
target_link_libraries(${PROJECT_NAME} base)

set(
    CLIENT_DEMO_SRC
    src/client/client_main.cpp
    src/protocol/message.h
    src/protocol/message.cpp
)
add_executable(client_demo ${CLIENT_DEMO_SRC})
target_link_libraries(client_demo base)