cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.31)
project(GLRenderer VERSION 1.0 LANGUAGES CXX C)
cmake_policy(SET CMP0074 NEW)

find_package(OpenGL REQUIRED)

# Set default build type if not set
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode")
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
endif()

# Compiler flags
set(PROJECT_WARNING_FLAGS "")
foreach(OUTPUTCONFIG IN ITEMS Debug Release)
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    if(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "/W4 /WX /Zi /Od")
        set(CMAKE_CXX_FLAGS_RELEASE "/W4 /WX /O2")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
        set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Werror -g -O0")
        set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -Werror -O3")
    endif()
endforeach()

if(MSVC)
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(PROJECT_WARNING_FLAGS /W4 /WX)
else()
    set(PROJECT_WARNING_FLAGS -Wall -Wextra -Werror)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path configuration
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

# GLAD as OBJECT library
add_library(glad_objects OBJECT "${SRC_DIR}/glad/glad.c")
set_property(TARGET glad_objects PROPERTY COMPILE_DEFINITIONS "GLAD_GL_IMPLEMENTATION")
target_include_directories(glad_objects PRIVATE "${EXTERNAL_INCLUDE_DIR}")

# Project source files
set(PROJECT_SOURCES
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/Engine.cpp"
    "${SRC_DIR}/GLRenderer/GLRenderer.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/OpenGLInitializer.cpp"
    "${SRC_DIR}/GLRenderer/Window/GLFW_Window.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/Scenes/C1/HelloTriangle.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/Scenes/C1/TexturedQuad.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/Scenes/C1/Quad3D.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/Scenes/C2/LightingCubes.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/GLMeshBuffers.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/GLShader.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/GLTexture.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/GLRenderState.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/GLSceneManager.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/GLCamera.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/GLSceneInputHandler.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/GLMesh.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/Lighting/GLMaterial.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/Lighting/GLLightManager.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/Lighting/Types/GLPointLight.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/Lighting/Types/GLSpotLight.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/Lighting/Types/GLDirLight.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/GLMesh.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/GLModel.cpp"
    "${SRC_DIR}/GLRenderer/OpenGL/Scenes/C3/ModelLoading.cpp"
    "${SRC_DIR}/stb_image/stb_image.cpp"
)

# External dependencies
include(FetchContent)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable tests" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable docs" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.89.8
)
FetchContent_MakeAvailable(imgui)

# Assimp options — set BEFORE FetchContent_Declare!
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable Assimp tests" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable Assimp tools" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "No Assimp install" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "No debug postfix" FORCE)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v6.0.2
)
FetchContent_MakeAvailable(assimp)


set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
)

set(IMGUI_BACKENDS
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_BACKENDS})

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${glfw_SOURCE_DIR}/include
    "${EXTERNAL_INCLUDE_DIR}/glad"
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL glad_objects)

# Main executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set(RUNTIME_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_OUTPUT_DIR}"
)

# Copy resources
set(RESOURCE_OUTPUT_DIR "${RUNTIME_OUTPUT_DIR}/res")
set(RESOURCE_TIMESTAMP "${RESOURCE_OUTPUT_DIR}/res_copied.timestamp")

add_custom_command(
    OUTPUT "${RESOURCE_TIMESTAMP}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/GLRenderer/res"
        "${RESOURCE_OUTPUT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E touch "${RESOURCE_TIMESTAMP}"
    COMMENT "Copying resources folder next to executable"
)

add_custom_target(CopyResources ALL
    DEPENDS "${RESOURCE_TIMESTAMP}"
)
add_dependencies(${PROJECT_NAME} CopyResources)

# Include directories for your project
# In your target includes:
target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${INCLUDE_DIR}"
        "${EXTERNAL_INCLUDE_DIR}"
        "${EXTERNAL_INCLUDE_DIR}/GLFW"
        "${EXTERNAL_INCLUDE_DIR}/glad"
        $<TARGET_PROPERTY:assimp::assimp,INTERFACE_INCLUDE_DIRECTORIES>  # Assimp includes
)

get_target_property(ASSIMP_INCLUDES assimp::assimp INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "Assimp include dirs: ${ASSIMP_INCLUDES}")

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glad_objects
        glfw
        imgui
        assimp::assimp     # Link assimp target from FetchContent
)

# Platform-specific OpenGL links
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE
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

# Warnings
target_compile_options(${PROJECT_NAME} PRIVATE ${PROJECT_WARNING_FLAGS})
