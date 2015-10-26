cmake_minimum_required(VERSION 2.6)
project(TomChip8)

# Set an output directory for binaries 
set(BIN_DIR ${TomChip8_SOURCE_DIR}/bin)

# Bump up warning levels approprately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(include ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_executable(TomChip8 src/main.cpp src/cpu.cpp)
target_link_libraries(TomChip8 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

install(TARGETS TomChip8 RUNTIME DESTINATION ${BIN_DIR})
