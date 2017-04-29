cmake_minimum_required(VERSION 3.2)

project(bmgcc)

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
ELSE()
    add_definitions( -DNO_WMAIN_SUPPORT )
ENDIF()

include_directories(bmgcc)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE  "/MD /Ox /Ob2 /Oy /Oi /D NDEBUG")
    set(CMAKE_C_FLAGS_RELEASE  "/MD /Ox /Ob2 /Oy /Oi /D NDEBUG")
ELSE()
    set(CMAKE_CXX_FLAGS_RELEASE  "-march=corei7-avx -Wall -Ofast -mtune=corei7-avx -mfpmath=both -save-temps -fverbose-asm")
    set(CMAKE_CXX_FLAGS_DEBUG  "-march=corei7-avx -Wall -mtune=corei7-avx -mfpmath=both -save-temps -fverbose-asm")
ENDIF()

set(BMGCC bmgcc)
file(GLOB BM_SOURCE_FILES "${BMGCC}/*.c")

set(BMT bmt)
set(BMT_SOURCE_FILES
    ${BMT}/bmt.cpp
    ${BMT}/SearchTest.cpp
    ${BMT}/gtest-all.cc
    ${BMGCC}/bmsearch.c
    ${BMGCC}/eprintf.c
)

add_executable(bm ${BM_SOURCE_FILES})
add_executable(bmt ${BMT_SOURCE_FILES})