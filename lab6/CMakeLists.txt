cmake_minimum_required(VERSION 3.25)
project(lab6)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Lab6Application.cpp
        src/Lab6Application.hpp
        src/scripts/TaskScript.cpp
        src/scripts/TaskScript.hpp
        src/ui/Menu.cpp
        src/ui/Menu.hpp
        src/models/GarlicFigure.cpp
        src/models/GarlicFigure.hpp
)

add_custom_target(copy_lab6_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/resources"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lab6_resources"
)

add_dependencies(${PROJECT_NAME} copy_lab6_resources)

target_include_directories(${PROJECT_NAME} PUBLIC "src")
find_package(nfd REQUIRED)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        core
        nfd::nfd
)