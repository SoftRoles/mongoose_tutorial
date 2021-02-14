cmake_minimum_required(VERSION 3.0.0)
project(mongoose VERSION 0.1.0)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.c")

add_executable(mongoose main.c ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
