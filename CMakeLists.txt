cmake_minimum_required(VERSION 3.10)

project(PyGuiExample)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the C++ library directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpp_library)

# The library executable/library targets here if we wanted to build PyGui as a static/shared lib
# For simplicity in this project, we'll compile PyGui directly into the example.

# Add the example executable
add_executable(example_app 
    example/main.cpp 
    cpp_library/PyGui.cpp
)

# Optional: Add threading library support for Linux/Mac
find_package(Threads REQUIRED)
target_link_libraries(example_app Threads::Threads)
