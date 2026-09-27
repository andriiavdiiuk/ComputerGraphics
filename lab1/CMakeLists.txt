cmake_minimum_required(VERSION 3.25)
project(lab1)

add_executable(lab1 ${SOURCES}
        src/main.cpp
        src/Lab1Application.cpp
        src/Lab1Application.hpp
        src/models/Figure.hpp
        src/models/Figure.cpp
        src/ui/Menu.hpp
        src/ui/Menu.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "src")

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        core
)