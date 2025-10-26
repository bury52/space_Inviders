cmake_minimum_required(VERSION 4.0)
project(space_invaders)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(space_invaders main.cpp
        enum.h
        util.h
        Player.h
        Robot.h
        Robot.cpp)
target_link_libraries(space_invaders PRIVATE SFML::Graphics SFML::Audio)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/
        DESTINATION bin/resources
)