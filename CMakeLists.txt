cmake_minimum_required(VERSION 3.10)
project(Solitaire)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define source files
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(Solitaire ${SOURCES})

# Include header files
include_directories(include)

# Enable compiler warnings
if (MSVC)
    target_compile_options(Solitaire PRIVATE /W4)
else()
    target_compile_options(Solitaire PRIVATE -Wall -Wextra -pedantic)
endif()