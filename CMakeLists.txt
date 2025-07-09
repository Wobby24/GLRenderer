cmake_minimum_required(VERSION 3.16)
project(GLRenderer VERSION 1.0 LANGUAGES CXX C)

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
    "${SRC_DIR}/GLRenderer/Window/Window.cpp"
    "${SRC_DIR}/Engine.cpp")

message(STATUS "GLAD source file (now part of glad_objects): ${SRC_DIR}/glad/glad.c")

# Executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES}) # glad.c is NOT here directly anymore!


# Post-build copy: only valid after the target exists
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND cmd /c if exist "${CMAKE_SOURCE_DIR}\\GLRenderer\\res" xcopy /E /I /Y "${CMAKE_SOURCE_DIR}\\GLRenderer\\res" "$<TARGET_FILE_DIR:${PROJECT_NAME}>\\res"
)


# Includes for the main executable
target_include_directories(${PROJECT_NAME}
    PUBLIC 
        "${INCLUDE_DIR}"
        "${EXTERNAL_INCLUDE_DIR}"
        "${EXTERNAL_INCLUDE_DIR}/GLFW"
        "${EXTERNAL_INCLUDE_DIR}/glad" # Still needed for your other C++ files that include glad.h
)

# Link specific libraries by name (without .lib extension)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glad_objects # <--- Link the newly created GLAD object library
        "${EXTERNAL_LIB_DIR}/glfw3.lib"
        opengl32    
)

# Debug macro
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG_BUILD>
)