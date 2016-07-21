cmake_minimum_required(VERSION 2.8)

project(bmgcc)

add_definitions( -DNO_WMAIN_SUPPORT )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS "-Wall -Ofast -march=nocona -mtune=nocona -mfpmath=both")
set(CMAKE_C_FLAGS "-fexpensive-optimizations -Wall -Ofast -march=nocona -mtune=nocona -mfpmath=both")

set(BMGCC bmgcc)
set(SOURCE_FILES ${BMGCC}/bm.c ${BMGCC}/bmsearch.c ${BMGCC}/eprintf.c)

add_executable(bm ${SOURCE_FILES})