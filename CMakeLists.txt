cmake_minimum_required(VERSION 3.10)

project(Snake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Generate compile_commands.json (useful for clang tooling)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Include all external dependencies
include(Dependencies.cmake)  # Keep dependencies centralized

# Add subprojects
add_subdirectory(Core)   # Core engine / library
add_subdirectory(App)    # Executable / game