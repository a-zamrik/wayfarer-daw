cmake_minimum_required(VERSION 3.0)

project(pa_dev)

# Tell cmake where libraries can be found
link_directories(${CMAKE_SOURCE_DIR}/lib)

# These instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
file(GLOB_RECURSE sources src/*.cpp src/*.h)

# Specify a executable from the found source files
add_executable(pa_dev ${sources})

# Specify how to compile
target_compile_options(pa_dev PUBLIC)

# Link portaudio library to pa_dev
target_link_libraries(pa_dev portaudio_x64)