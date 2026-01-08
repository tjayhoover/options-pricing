cmake_minimum_required(VERSION 3.15)
project(options_pricing)
set(CMAKE_CXX_STANDARD 17)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Prefer FindPython mode for modern pybind11 integration (vcpkg provides CMake config)
set(Python_FIND_ABI "OFF" CACHE INTERNAL "")
find_package(Python COMPONENTS Interpreter Development)
add_library(options_pricing_lib STATIC
	src/variable_data.cpp
	src/variable.cpp
	src/operators.cpp
)
target_include_directories(options_pricing_lib PRIVATE include)

add_executable(options_pricing src/main.cpp)
target_link_libraries(options_pricing PRIVATE options_pricing_lib)
target_include_directories(options_pricing PRIVATE include)

# Optionally build Python bindings using pybind11 if available
find_package(pybind11 CONFIG)
if (pybind11_FOUND)
	# Use pybind11_add_module which sets proper include dirs and link flags
	pybind11_add_module(options_pricing_py src/python_bindings.cpp)
	target_link_libraries(options_pricing_py PRIVATE options_pricing_lib)
	target_include_directories(options_pricing_py PRIVATE include)
else()
	message(STATUS "pybind11 not found -- Python bindings will not be built.")
endif()