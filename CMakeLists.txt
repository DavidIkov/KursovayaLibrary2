cmake_minimum_required(VERSION 3.31.3)
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
    Graphics/Primitives/VertexBuffer.cpp
    Graphics/Primitives/VertexBuffer.hpp
    Graphics/Primitives/VertexArray.cpp
    Graphics/Primitives/VertexArray.hpp
    Graphics/Primitives/Texture.cpp
    Graphics/Primitives/Texture.hpp
    Graphics/Primitives/FrameBuffer.cpp
    Graphics/Primitives/FrameBuffer.hpp
    Graphics/Primitives/RenderBuffer.cpp
    Graphics/Primitives/RenderBuffer.hpp
    Graphics/Primitives/Shader.cpp
    Graphics/Primitives/Shader.hpp
    Graphics/Primitives/ShaderProgram.cpp
    Graphics/Primitives/ShaderProgram.hpp
    Graphics/Primitives/IndexBuffer.cpp
    Graphics/Primitives/IndexBuffer.hpp
    Graphics/Primitives/Renderer.cpp
    Graphics/Primitives/Renderer.hpp
    Graphics/Primitives/RenderingPreset.cpp
    Graphics/Primitives/RenderingPreset.hpp
    Graphics/Abstractions/TextRenderer.cpp
    Graphics/Abstractions/TextRenderer.hpp

)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standart")

target_include_directories(KL2 PRIVATE ${PROJECT_SOURCE_DIR})

#set(KL2_GLFW_DIR ${PROJECT_SOURCE_DIR} CACHE PATH "path to glfw library source")
if(NOT DEFINED GLFW_LIB_DIR)
    message(FATAL_ERROR "glfw library source directory was not provided")
else()
    target_include_directories(KL2 PRIVATE ${GLFW_LIB_DIR})
    add_subdirectory(${GLFW_LIB_DIR}/glfw glfw)
    target_link_libraries(KL2 PRIVATE glfw)
endif()

if(NOT DEFINED GLAD_LIB_DIR)
    message(FATAL_ERROR "glad library source directory was not provided")
else()
    target_include_directories(KL2 PRIVATE ${GLAD_LIB_DIR})
    add_subdirectory(${GLAD_LIB_DIR}/glad glad)
    target_link_libraries(KL2 PRIVATE glad)
endif()


if(NOT DEFINED STB_IMAGE_LIB_DIR)
    message(WARNING "stb_image library source directory was not provided, ignoring functions that use stb_image")
    add_compile_definitions(KL2_DONT_USE_STB_IMAGE)
else()
    target_include_directories(KL2 PRIVATE ${STB_IMAGE_LIB_DIR})
    add_subdirectory(${STB_IMAGE_LIB_DIR}/stb_image stb_image)
    target_link_libraries(KL2 PRIVATE stb_image)
endif()

if(NOT DEFINED FREE_TYPE_LIB_DIR)
    message(WARNING "FreeType library source directory was not provided, ignoring functions that use FreeType")
    add_compile_definitions(KL2_DONT_USE_FREE_TYPE)
else()
    target_include_directories(KL2 PRIVATE ${FREE_TYPE_LIB_DIR})
    add_subdirectory(${FREE_TYPE_LIB_DIR}/FreeType freetype)
    target_link_libraries(KL2 PRIVATE freetype)
endif()
