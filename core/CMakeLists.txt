cmake_minimum_required(VERSION 3.25)
project(core)

add_library(core ${SOURCES}
        src/primitives/Grid.hpp
        src/primitives/Grid.cpp
        src/primitives/Line.hpp
        src/utils/FileUtils.cpp
        src/utils/FileUtils.hpp
        src/graphics/meshes/Mesh.cpp
        src/graphics/meshes/Mesh.hpp
        src/graphics/buffers/IndexBufferObject.hpp
        src/graphics/buffers/VertexArrayObject.hpp
        src/graphics/buffers/VertexBufferObject.hpp
        src/primitives/Line.cpp
        src/primitives/Circle.hpp
        src/primitives/Circle.cpp
        src/transformations/Transformation.hpp
        src/transformations/Transformation.cpp
        src/Application.hpp
        src/Application.cpp
        src/math/Derivative.cpp
        src/math/Derivative.hpp
)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/resources"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources"
)

add_dependencies(${PROJECT_NAME} copy_resources)


target_include_directories(${PROJECT_NAME} PUBLIC "src")

find_package(glad REQUIRED)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(ImGui-SFML REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        glad::glad
        SFML::Graphics
        SFML::Window
        SFML::System
        glm::glm
        OpenGL::GL
        imgui::imgui
        spdlog::spdlog
        ImGui-SFML::ImGui-SFML
)