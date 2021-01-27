cmake_minimum_required(VERSION 3.9)
project(tlvpp)
include(CheckTypeSize)

set(CMAKE_CXX_STANDARD 11)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
#include(ConanInit)
#conan_init()

add_library(tlv_c
            STATIC
            include/key_list.h
            include/tlv_box.h
            src/key_list.c
            src/tlv_box.c
            )
set_target_properties(tlv_c PROPERTIES LANGUAGES C)
set_target_properties(tlv_c PROPERTIES C_STANDARD 99)

add_library(tlvpp
            STATIC
            include/tlvpp.h
            src/tlvpp.cpp)

target_link_libraries(tlvpp tlv_c)
set_target_properties(tlvpp PROPERTIES LANGUAGES CXX)
set_target_properties(tlvpp PROPERTIES CXX_STANDARD 11)