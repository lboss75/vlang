project(vruntime CXX C)
cmake_minimum_required(VERSION 2.6.2)

FILE(GLOB HEADER_FILES *.h)
FILE(GLOB SOURCE_FILES *.cpp)

ADD_LIBRARY(vruntime STATIC ${SOURCE_FILES} ${HEADER_FILES})

