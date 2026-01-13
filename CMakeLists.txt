cmake_minimum_required(VERSION 4.0)
project(space_invaders)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

add_executable(space_invaders main.cpp
        enum.h
        util.h
        Player.h
        Robot.h
        Robot.cpp
        concepts.h
        structs.h
        Bullet.h
        Bullet_Controller.h
        Wall.h
        Button.h
        TomlReader.h
        StartMenu.h
        Game.h
        Lazy.h)
target_link_libraries(space_invaders PRIVATE SFML::Graphics SFML::Audio tomlplusplus::tomlplusplus)

add_custom_command(TARGET space_invaders POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:space_invaders>/resources
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/
        DESTINATION bin/resources
)

install(TARGETS space_invaders
        RUNTIME DESTINATION bin
)