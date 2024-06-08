cmake_minimum_required(VERSION 3.0)

# Should we compile with ImGui as the GUI framework?
option(USE_IMGUI "Compile to use ImGui" OFF)

# If using ImGui, set these to the desired backends you want to use.
# You will have to change the setup code in wayfarers GUI class if you are not using
# DX12/Windows.
# The GUI class is based off of the exmaple code provied by Dear ImGui.
# Use how it is set up now as a reference for a different backend; 
# Just need the initalize and clean up code essentially.
if (USE_IMGUI)
    set(IMGUI_BACKEND_REGEX "imgui/backends/*win32*" "imgui/backends/*dx12*")
    if (IMGUI_BACKEND_REGEX MATCHES ".*win32.*")
        add_compile_definitions(USE_IMGUI_WINDOWS) # used by preprocessor compile guard
    endif()
    if (IMGUI_BACKEND_REGEX MATCHES ".*dx12.*")
        add_compile_definitions(USE_IMGUI_DX12) # used by preprocessor compile guard
    endif()
endif()


set(TARGET_NAME "wayfarer")
project(wayfarer)

# Tell cmake where libraries can be found
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Tell cmake where "pre-defined" includes i.e. #include <header.h>
include_directories(include)

# Add wayfarer source files
file(GLOB_RECURSE sources src/*.cpp src/*.h)


if (USE_IMGUI)
    add_compile_definitions(USE_IMGUI)  # Create c/c++ preprocessor define.
                                        # Used to create preprocessor compile guards
    include_directories(imgui)
    include_directories(imgui/backends)

    # Add only the ImGui files we will need. Misc, Examples, ..., are not needed
    file(GLOB ImGui_sources imgui/*.cpp imgui/*.h)

    # Specify the needed backend files
    file(GLOB ImGui_backend_sources ${IMGUI_BACKEND_REGEX})

    # Add ImGui source code to wayfarer's list
    list (APPEND sources ${ImGui_sources} ${ImGui_backend_sources})
endif()

# Specify a executable from the found source files
add_executable(${TARGET_NAME} ${sources})

# Specify how to compile
target_compile_options(${TARGET_NAME} PUBLIC)

# Link all required libraries to wayfarer
target_link_libraries(${TARGET_NAME} portaudio_x64 d3d12.lib d3dcompiler.lib dxgi.lib freetype)

