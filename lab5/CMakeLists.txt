cmake_minimum_required(VERSION 3.25)
project(lab5)

add_executable(lab5
        src/main.cpp
        src/Lab5Application.cpp
        src/Lab5Application.hpp
        src/scripts/TaskScript.cpp
        src/scripts/TaskScript.hpp
        src/ui/Menu.cpp
        src/ui/Menu.hpp
        src/models/Figure.cpp
        src/models/Figure.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "src")
find_package(nfd REQUIRED)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        core
        nfd::nfd
)