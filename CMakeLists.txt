cmake_minimum_required(VERSION 3.25)
project(ProjectC)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)

FetchContent_MakeAvailable(SFML)
add_executable(ProjectC
        main.cpp
        Game.cpp
        Settings.cpp
        Settings.h
        UIComponents.cpp
        UIComponents.h
)
target_link_libraries(ProjectC sfml-graphics sfml-audio)
add_custom_command(
        TARGET ProjectC
        COMMENT "copy stuff"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:ProjectC>
        VERBATIM
)