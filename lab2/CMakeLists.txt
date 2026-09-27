cmake_minimum_required(VERSION 3.25)
project(lab2)

add_executable(lab2 ${SOURCES}
        src/main.cpp
        src/Lab2Application.cpp
        src/Lab2Application.hpp
        src/models/FoliumOfDescartes.cpp
        src/models/FoliumOfDescartes.hpp
        src/ui/Menu.cpp
        src/ui/Menu.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "src")

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        core
)