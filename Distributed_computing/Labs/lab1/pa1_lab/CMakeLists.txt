project(pa1_lab)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -Wall -pedantic")

set(SOURCE_FILES common.h ipc.h pa1.h main.c)

add_executable(pa1_lab ${SOURCE_FILES})