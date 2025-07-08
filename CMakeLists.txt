cmake_minimum_required(VERSION 3.16)
project(GLRenderer VERSION 1.0 LANGUAGES CXX)

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

# Add sources to project
set(PROJECT_SOURCES
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/glad/glad.c"
)

# Executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Post-build copy: only valid after the target exists
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND cmd /c if exist "${CMAKE_SOURCE_DIR}\\GLRenderer\\res" xcopy /E /I /Y "${CMAKE_SOURCE_DIR}\\GLRenderer\\res" "$<TARGET_FILE_DIR:${PROJECT_NAME}>\\res"
)


# Includes
target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${INCLUDE_DIR}"
        "${EXTERNAL_INCLUDE_DIR}"
        "${EXTERNAL_INCLUDE_DIR}/GLFW"
        "${EXTERNAL_INCLUDE_DIR}/glad"
)

# Link specific libraries by name (without .lib extension)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        "${EXTERNAL_LIB_DIR}/glfw3.lib"
)

# Debug macro
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG_BUILD>
)




