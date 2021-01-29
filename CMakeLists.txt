cmake_minimum_required(VERSION 3.9)
project(tlvpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY lib)

set(CMAKE_CXX_STANDARD 11)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
#include(ConanInit)
#conan_init()

#add_definitions(-DTLVPP_FORCE_BIG_ENDIAN)

set(TLV_C_SOURCES
    include/key_list.h
    include/tlv_box.h
    src/key_list.c
    src/tlv_box.c
    include/data.h
    include/byte_order.h
    )

if (WIN32)
	set(TLV_C_SOURCES
	    ${TLV_C_SOURCES}
	    include/win_endian.h
	    )
endif ()
add_library(tlv_c
            STATIC
            ${TLV_C_SOURCES}
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

if (TLV_TEST)
	set(gtest_force_shared_crt ${ENABLE_SHARED} CACHE BOOL "" FORCE)
	set(PROJECT_NAME_TEST ${PROJECT_NAME}-test)

	if (APPLE)
		add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
		add_definitions(-D__GLIBCXX__)
	endif ()
	if (MSVC)
		string(REGEX REPLACE "\\/W4" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	endif ()

	#gtest
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest)
	add_library(CONAN_PKG::gtest ALIAS gtest)

	enable_testing()

	set(TEST_SOURCES
	    tests/main.cpp
	    tests/tlvpp.cpp
	    )

	add_executable(${PROJECT_NAME_TEST} ${TEST_SOURCES})
	if (NOT MSVC)
		target_compile_options(${PROJECT_NAME_TEST} PUBLIC -Wno-unused-parameter -fno-omit-frame-pointer -g)
	endif ()
	target_include_directories(${PROJECT_NAME_TEST} PRIVATE tests)
	target_include_directories(${PROJECT_NAME_TEST} PRIVATE include)

	target_link_libraries(${PROJECT_NAME_TEST} PRIVATE CONAN_PKG::gtest)
	target_link_libraries(${PROJECT_NAME_TEST} PUBLIC ${PROJECT_NAME})

	add_test(
		NAME test-all
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/${PROJECT_NAME_TEST}
	)

	#	set(CMAKE_EXE_LINKER_FLAGS -fuse-ld=/usr/bin/clang++)
	#set(CMAKE_VERBOSE_MAKEFILE ON)
	#	set(CMAKE_C_LINK_EXECUTABLE /usr/bin/clang)
	#	set(CMAKE_CXX_LINK_EXECUTABLE /usr/bin/clang++)
	#	add_executable(ext-test tests/ext_test.cpp)
	#	target_compile_options(ext-test PUBLIC -Wno-unused-parameter -fsanitize=undefined -fno-omit-frame-pointer -g)
	#	target_include_directories(ext-test PRIVATE include)
	#
	##	target_link_libraries(ext-test asan)
	#	target_link_libraries(ext-test ubsan)
	#	target_link_libraries(ext-test ${PROJECT_NAME})

endif ()