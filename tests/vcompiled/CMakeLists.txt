project(vcompiled CXX C)
cmake_minimum_required(VERSION 2.6.2)

include_directories(${vruntime_SOURCE_DIR})

FILE(GLOB HEADER_FILES *.h)
FILE(GLOB SOURCE_FILES *.cpp)

add_executable(vcompiled ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(vcompiled vruntime ${CMAKE_THREAD_LIBS_INIT})
