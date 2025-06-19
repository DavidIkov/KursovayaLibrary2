cmake_minimum_required(VERSION 3.28.3)
project(KL2 LANGUAGES C CXX)

add_library(KL2 STATIC
    KL2_Manager.cpp
    KL2_Manager.hpp
    Tools/ErrorsSystem.cpp
    Tools/ErrorsSystem.hpp
    Tools/Event.cpp
    Tools/Event.hpp
    Window/Window.cpp
    Window/Window.hpp
    Input/Keyboard.cpp
    Input/Keyboard.hpp
    Input/Mouse.cpp
    Input/Mouse.hpp
    Graphics/VertexBuffer.cpp
    Graphics/VertexBuffer.hpp
    Graphics/VertexArray.cpp
    Graphics/VertexArray.hpp
    Graphics/Texture.cpp
    Graphics/Texture.hpp
    Graphics/FrameBuffer.cpp
    Graphics/FrameBuffer.hpp
    Graphics/RenderBuffer.cpp
    Graphics/RenderBuffer.hpp
    Graphics/Shader.cpp
    Graphics/Shader.hpp
    Graphics/ShaderProgram.cpp
    Graphics/ShaderProgram.hpp
    Graphics/IndexBuffer.cpp
    Graphics/IndexBuffer.hpp
    Graphics/Renderer.cpp
    Graphics/Renderer.hpp
    Graphics/RenderingPreset.cpp
    Graphics/RenderingPreset.hpp
    Graphics/TextRenderer.cpp
    Graphics/TextRenderer.hpp

)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standart")

target_link_libraries(KL2 PUBLIC stdc++)

target_include_directories(KL2 PRIVATE ${PROJECT_SOURCE_DIR})

set(KL2_GLFW_DIR ${PROJECT_SOURCE_DIR}/libs/glfw)
if(NOT EXISTS ${KL2_GLFW_DIR})
    message(FATAL_ERROR "glfw library not found")
else()
    target_include_directories(KL2 PRIVATE ${KL2_GLFW_DIR}/..)
    add_subdirectory(${KL2_GLFW_DIR} glfw)
    target_link_libraries(KL2 PRIVATE glfw)
endif()

set(KL2_GLAD_DIR ${PROJECT_SOURCE_DIR}/libs/glad)
if(NOT EXISTS ${KL2_GLAD_DIR})
    message(FATAL_ERROR "glad library not found")
else()
    target_include_directories(KL2 PRIVATE ${KL2_GLAD_DIR}/..)
    add_subdirectory(${KL2_GLAD_DIR} glad)
    target_link_libraries(KL2 PRIVATE glad)
endif()

set(KL2_STB_IMAGE_DIR ${PROJECT_SOURCE_DIR}/libs/stb_image)
set(KL2_IGNORE_STB_IMAGE FALSE CACHE BOOL "KL2_IGNORE_STB_IMAGE")
if(${KL2_IGNORE_STB_IMAGE} STREQUAL TRUE)
    add_compile_definitions(KL2_DONT_USE_STB_IMAGE)
elseif(NOT EXISTS ${KL2_STB_IMAGE_DIR})
    message(WARNING "stb_image library not found, ignoring functions that use stb_image")
    add_compile_definitions(KL2_DONT_USE_STB_IMAGE)
else()
    target_include_directories(KL2 PRIVATE ${KL2_STB_IMAGE_DIR}/..)
    add_subdirectory(${KL2_STB_IMAGE_DIR} stb_image)
    target_link_libraries(KL2 PRIVATE stb_image)
endif()

set(KL2_FREE_TYPE_DIR ${PROJECT_SOURCE_DIR}/libs/FreeType)
set(KL2_IGNORE_FREE_TYPE FALSE CACHE BOOL "KL2_IGNORE_FREE_TYPE")
if(${KL2_IGNORE_FREE_TYPE} STREQUAL TRUE)
    add_compile_definitions(KL2_DONT_USE_FREE_TYPE)
elseif(NOT EXISTS ${KL2_FREE_TYPE_DIR})
    message(WARNING "FreeType library source directory was not provided, ignoring functions that use FreeType")
    add_compile_definitions(KL2_DONT_USE_FREE_TYPE)
else()
    target_include_directories(KL2 PRIVATE ${KL2_FREE_TYPE_DIR}/..)
    add_subdirectory(${KL2_FREE_TYPE_DIR} freetype)
    target_link_libraries(KL2 PRIVATE freetype)
endif()
