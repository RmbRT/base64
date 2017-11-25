cmake_minimum_required(VERSION 2.6)
project(base64)

# Options for configuring the project.
option(OPENMP "Enable OpenMP support" OFF)
option(DEBUG "Generate Debug symbols" ON)
option(ASSERTIONS "Enable assertions" ON)
option(OPTIMIZE "Enable optimization" ON)

# OpenMP support?
if(OPENMP)
	set(flags "-fopenmp")
else()
	set(flags "-Wno-unknown-pragmas")
endif()

# Assertions?
if(NOT ASSERTIONS)
	set(flags "${flags} -DNDEBUG")
endif()

# Debug symbols?
if(DEBUG)
	set(flags "${flags} -g")
endif()

# Optimize?
if(OPTIMIZE)
	set(flags "${flags} -O3")
endif()


# Build flags.
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror ${flags} -Wfatal-errors")

# Select all source files.
file(GLOB_RECURSE base64_sources ./src/*.cpp)

add_library(base64 STATIC ${base64_sources})

# copy headers into the include directory.
file(COPY "src/" DESTINATION "include/base64/" FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inl")