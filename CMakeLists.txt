﻿cmake_minimum_required(VERSION 3.21)

project(bm)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
ELSE()
    add_definitions( -DNO_WMAIN_SUPPORT )
ENDIF()

include_directories(bm)

include(GoogleTest)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE  "/MP /Zc:wchar_t /Zc:inline /MT /Ox /Ob2 /Oy /Oi /Ot /GL /D NDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG  "/MP /Zc:wchar_t /Zc:inline /MTd /Od /RTC1")
    set(CMAKE_C_FLAGS_RELEASE  "/MP /Zc:wchar_t /Zc:inline /MT /Ox /Ob2 /Oy /Oi /Ot /GL /D NDEBUG")
    set(CMAKE_C_FLAGS_DEBUG  "/MP /Zc:wchar_t /Zc:inline /MTd /Od /RTC1")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
        set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /LTCG")
    endif()
else()
    set(CMAKE_CXX_FLAGS_RELEASE  "-march=corei7-avx -Wall -Ofast -mtune=corei7-avx -mfpmath=both -save-temps -fverbose-asm -std=c++14")
    set(CMAKE_CXX_FLAGS_DEBUG  "-march=corei7-avx -Wall -mtune=corei7-avx -mfpmath=both -save-temps -fverbose-asm -std=c++14")
endif()

set(BM bm)
file(GLOB BM_SOURCE_FILES "${BM}/*.c")

set(BMT bmt)
set(BMT_SOURCE_FILES
    ${BMT}/bmt.cpp
    ${BMT}/search_test.cpp
    ${BM}/bmsearch.c
    ${BM}/eprintf.c
)

add_executable(bm ${BM_SOURCE_FILES})
add_executable(bmt ${BMT_SOURCE_FILES})

if(UNIX)
target_link_libraries(bm m)
target_link_libraries(bmt m pthread GTest::gtest_main)
else()
target_link_libraries(bmt GTest::gtest_main)
endif()
