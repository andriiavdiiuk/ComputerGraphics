cmake_minimum_required(VERSION 3.25)
project(lab7)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Lab7Application.cpp
        src/Lab7Application.hpp
        src/scripts/TaskScript.hpp
        src/scripts/TaskScript.cpp
        src/scripts/ScreenMovementScript.hpp
        src/scripts/ScreenMovementScript.cpp
        src/models/IfsFractal.hpp
        src/Models/IfsFractal.cpp
        src/ui/Menu.hpp
        src/ui/Menu.cpp
)


add_custom_target(copy_lab7_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/resources"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lab7_resources"
)

add_dependencies(${PROJECT_NAME} copy_lab7_resources)

target_include_directories(${PROJECT_NAME} PUBLIC "src")

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        core
)