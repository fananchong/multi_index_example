CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

PROJECT(multi_index_example)

MESSAGE(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})

ADD_DEFINITIONS(-std=c++11)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/contrib/boost/include/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/example)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/)

SET(EXECUTABLE_OUTPUT_PATH ~/multi_index_example/bin)

ADD_SUBDIRECTORY(example1)
ADD_SUBDIRECTORY(example2)
ADD_SUBDIRECTORY(example3)
