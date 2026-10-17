cmake_minimum_required(VERSION 3.25)
project(lab3)

add_executable(lab3 ${SOURCES}
        src/main.cpp
        src/Lab3Application.hpp
        src/Lab3Application.cpp
        src/scripts/CurveScript.cpp
        src/scripts/CurveScript.cpp
        src/scripts/ScreenMovementScript.cpp
        src/scripts/ScreenMovementScript.hpp
        src/ui/Menu.cpp
        src/ui/Menu.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "src")
find_package(nfd REQUIRED)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        core
        nfd::nfd
)