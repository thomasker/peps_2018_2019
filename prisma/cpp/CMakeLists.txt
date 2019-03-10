# Definitions
cmake_minimum_required(VERSION 3.0)
project(prisma-cpp)

# PNL Package
set(Pnl_DIR "pnl/build/lib/cmake/pnl")
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})

# Add math lib
set(LIBS ${LIBS} m)

# Add sources
file(GLOB LIB_SOURCES src/*.cpp)
file(GLOB LIB_HEADERS src/*.h)
add_library(prisma-cpp SHARED ${LIB_SOURCES} ${LIB_HEADERS})