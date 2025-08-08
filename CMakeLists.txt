cmake_minimum_required(VERSION 3.16)
project(GLRenderer VERSION 1.0 LANGUAGES CXX C)
cmake_policy(SET CMP0074 NEW)
find_package(OpenGL REQUIRED)

# Set warning flags per compiler
set(PROJECT_WARNING_FLAGS "")

if (MSVC)
    # Remove existing /W flags
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(PROJECT_WARNING_FLAGS /W4 /WX)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(PROJECT_WARNING_FLAGS -Wall -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(PROJECT_WARNING_FLAGS -Wall -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNING_FLAGS -Wall -Wextra -Werror)
else()
    message(WARNING "Unknown compiler, warnings as errors not enabled")
endif()


# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(SRC_DIR "${CMAKE_SOURCE_DIR}/GLRenderer/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/GLRenderer/inc")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/GLRenderer/lib")
set(EXTERNAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/GLRenderer/ext/inc")
set(EXTERNAL_LIB_DIR "${CMAKE_SOURCE_DIR}/GLRenderer/ext/lib")
if(WIN32)
    set(EXTERNAL_LIB_PLATFORM_DIR "${EXTERNAL_LIB_DIR}/windows")
elseif(APPLE)
    set(EXTERNAL_LIB_PLATFORM_DIR "${EXTERNAL_LIB_DIR}/macos")
elseif(UNIX)
    set(EXTERNAL_LIB_PLATFORM_DIR "${EXTERNAL_LIB_DIR}/linux")
endif()


# ----------------------------------------------------
# Define GLAD as an OBJECT library
add_library(glad_objects OBJECT "${SRC_DIR}/glad/glad.c")

# Set compile definitions for the GLAD object library
set_property(TARGET glad_objects PROPERTY COMPILE_DEFINITIONS "GLAD_GL_IMPLEMENTATION")

# Make sure GLAD's own header is found when compiling glad.c
target_include_directories(glad_objects PRIVATE "${EXTERNAL_INCLUDE_DIR}")
# ----------------------------------------------------


# Add remaining sources to project (EXCLUDING glad.c now)
set(PROJECT_SOURCES
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/GLRenderer/GLRenderer.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/OpenGLInitializer.cpp"
    "GLRenderer/src/GLRenderer/Window/GLFW_Window.cpp"
    "${SRC_DIR}/Engine.cpp" 
    "${SRC_DIR}/GLRenderer/OpenGL/Scenes/C1/HelloTriangle.cpp" 
    "GLRenderer/src/GLRenderer/OpenGL/GLMeshBuffers.cpp" 
    "GLRenderer/src/GLRenderer/OpenGL/GLShader.cpp" 
    "GLRenderer/src/GLRenderer/OpenGL/Scenes/C1/TexturedQuad.cpp" 
    "GLRenderer/src/GLRenderer/OpenGL/GLTexture.cpp" 
    "GLRenderer/src/stb_image/stb_image.cpp" 
    "GLRenderer/src/GLRenderer/OpenGL/Scenes/C1/Quad3D.cpp" "GLRenderer/src/GLRenderer/OpenGL/GLRenderState.cpp" "GLRenderer/src/GLRenderer/OpenGL/GLSceneManager.cpp" "GLRenderer/src/GLRenderer/OpenGL/GLCamera.cpp" "GLRenderer/src/GLRenderer/OpenGL/GLSceneInputHandler.cpp")

include(FetchContent)

# GLFW options: adjust depending on OS
if(UNIX AND NOT APPLE)
    # Assume Linux
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
endif()

# Don't build tests/examples/docs
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8  # or latest stable version
)

FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

# Executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES}) # glad.c is NOT here directly anymore!

target_compile_options(${PROJECT_NAME} PRIVATE ${PROJECT_WARNING_FLAGS})

# Post-build copy: only valid after the target exists
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/GLRenderer/res"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
)


# Includes for the main executable
target_include_directories(${PROJECT_NAME}
    PUBLIC 
        "${INCLUDE_DIR}"
        "${EXTERNAL_INCLUDE_DIR}"
        "${EXTERNAL_INCLUDE_DIR}/GLFW"
        "${EXTERNAL_INCLUDE_DIR}/glad" # Still needed for your other C++ files that include glad.h
)

# Link GLAD
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glad_objects
)

# Platform-specific linking
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            glfw
            opengl32
    )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            glfw
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            glfw
            OpenGL::GL
            dl
            pthread
            X11
            Xrandr
            Xi
            Xxf86vm
            Xcursor
    )
endif()

# Debug macro
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG_BUILD>
)