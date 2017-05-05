project(v CXX C)
cmake_minimum_required(VERSION 2.6.2)

include_directories(${vruntime_SOURCE_DIR})
include_directories(${vcompiler_SOURCE_DIR})
include_directories(${v2cpp_SOURCE_DIR})

FILE(GLOB HEADER_FILES *.h)
FILE(GLOB SOURCE_FILES *.cpp)
ADD_MSVC_PRECOMPILED_HEADER("stdafx.h" "stdafx.cpp" SOURCE_FILES)

add_executable(v ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(v vcompiler vruntime v2cpp ${CMAKE_THREAD_LIBS_INIT})


