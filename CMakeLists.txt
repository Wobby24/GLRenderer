cmake_minimum_required(VERSION 3.16)
project(VR_GAME_CODE VERSION 1.0 LANGUAGES C CXX)

# ------------------------
# C++ settings
# ------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

# ------------------------
# OpenGL
# ------------------------
find_package(OpenGL REQUIRED)

# ------------------------
# Paths
# ------------------------
set(SRC_DIR "${CMAKE_SOURCE_DIR}/Engine/src/Engine")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Engine/inc/Engine")

# ------------------------
# GLAD (object library)
# ------------------------
add_library(glad OBJECT
    "${CMAKE_SOURCE_DIR}/Engine/src/Dependencies/glad/glad.c"
)

target_include_directories(glad PUBLIC
    "${CMAKE_SOURCE_DIR}/Engine/ext/inc"
)

# ------------------------
# FetchContent
# ------------------------
include(FetchContent)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.89.8
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL glad)

# Assimp
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.2
)
FetchContent_MakeAvailable(assimp)

# ------------------------
# Source files
# ------------------------
set(RENDERER_SRC 
"${SRC_DIR}/Renderer/OpenGL/GLRenderer.cpp" 
"${SRC_DIR}/Renderer/OpenGL/OpenGLInitializer.cpp" 
"${SRC_DIR}/Renderer/OpenGL/GLMeshBuffers.cpp" 
"${SRC_DIR}/Renderer/OpenGL/GLShader.cpp" 
"${SRC_DIR}/Renderer/OpenGL/GLTexture.cpp" 
"${SRC_DIR}/Renderer/OpenGL/GLRenderState.cpp" 
"${SRC_DIR}/Renderer/OpenGL/GLCamera.cpp" 
"${SRC_DIR}/Renderer/OpenGL/GLMesh.cpp" 
"${SRC_DIR}/Renderer/OpenGL/Lighting/GLMaterial.cpp" 
#"${SRC_DIR}/Renderer/OpenGL/Lighting/GLLightManager.cpp" 
"${SRC_DIR}/Renderer/OpenGL/Lighting/Types/GLPointLight.cpp" 
"${SRC_DIR}/Renderer/OpenGL/Lighting/Types/GLSpotLight.cpp" 
"${SRC_DIR}/Renderer/OpenGL/Lighting/Types/GLDirLight.cpp" 
"${SRC_DIR}/Renderer/OpenGL/GLMesh.cpp" 
"${SRC_DIR}/Renderer/OpenGL/GLModel.cpp" 
#"${SRC_DIR}/Renderer/OpenGL/Scenes/C3/ModelLoading.cpp" 
"${SRC_DIR}/Renderer/OpenGL/GLTextureCache.cpp" 
"${SRC_DIR}/Renderer/OpenGL/GLTextureBinder.cpp" 
#"${SRC_DIR}/Renderer/OpenGL/GLFPSCamController.cpp"
)
# Platform source files 
set(PLATFORM_SRC 
"${SRC_DIR}/Platform/RenderSurface/GLFW_RenderSurface.cpp" 
"${SRC_DIR}/Platform/Window/GLFW_Window.cpp" 
"${SRC_DIR}/Platform/Input/GLFW_InputService.cpp"
) 

set(DEPENDENCIES_SRC 
"${CMAKE_SOURCE_DIR}/Engine/src/Dependencies/stb_image/stb_image.cpp"
)

set(ENGINE_SRC
"${SRC_DIR}/Engine/Scene/ModelScene.cpp"
"${SRC_DIR}/Engine/VR_Engine.cpp"
)

# ------------------------
# Executable
# ------------------------
# Engine library
add_library(VR_Engine STATIC
    ${RENDERER_SRC}
    ${PLATFORM_SRC}
    ${DEPENDENCIES_SRC}
    ${ENGINE_SRC}
    $<TARGET_OBJECTS:glad>
)

target_include_directories(VR_Engine PUBLIC
    "${INCLUDE_DIR}"
)

target_compile_features(VR_Engine PUBLIC cxx_std_20)

set_target_properties(VR_Engine PROPERTIES
    LINKER_LANGUAGE CXX
)

target_link_libraries(VR_Engine PUBLIC
    glfw
    imgui
    assimp
    OpenGL::GL
)

# Game executable
add_executable(${PROJECT_NAME}
    "${CMAKE_SOURCE_DIR}/Engine/src/main.cpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE VR_Engine)


# ------------------------
# Warnings
# ------------------------
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
endif()
