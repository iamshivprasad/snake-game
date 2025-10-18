# Dependencies.cmake

find_package(SDL3 CONFIG REQUIRED)

# Create an interface library to bundle all external deps
add_library(ExternalLibs INTERFACE)

target_link_libraries(ExternalLibs
    INTERFACE
        SDL3::SDL3
)