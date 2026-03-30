# Dependencies.cmake

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf REQUIRED)


# Create an interface library to bundle all external deps
add_library(ExternalLibs INTERFACE)

target_link_libraries(ExternalLibs
    INTERFACE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
)        