#
# Snake-game
#

set(SOURCES
    Source/Main.cpp
)

add_executable(Snake-game)

target_sources(Snake-game PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

target_link_libraries(Snake-game
    PRIVATE
        ExternalLibs
)

target_include_directories(Snake-game PRIVATE Source)

# Set working directory for VS debugger
set_target_properties(Snake-game PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)