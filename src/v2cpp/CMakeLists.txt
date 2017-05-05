project(v2cpp CXX C)
cmake_minimum_required(VERSION 2.6.2)

include_directories(${vruntime_SOURCE_DIR})

FILE(GLOB HEADER_FILES *.h)
FILE(GLOB SOURCE_FILES *.cpp)

ADD_MSVC_PRECOMPILED_HEADER("stdafx.h" "stdafx.cpp" SOURCE_FILES)

ADD_LIBRARY(v2cpp STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(v2cpp vruntime)

