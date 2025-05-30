cmake_minimum_required(VERSION 3.31)

project(PokeRemake)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Source/Game)
add_subdirectory(Source/Runtime)


# Third Party Libraries

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")

# Glad
include_directories(ThirdParty)

add_executable(${PROJECT_NAME} Source/Game/Main.cpp)
target_link_libraries(
        ${PROJECT_NAME} PUBLIC PokeRemakeGame)
target_link_libraries(
        ${PROJECT_NAME} PUBLIC PokeRemakeRuntime
)

# Set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../$<CONFIGURATION>")

# SDL3
add_subdirectory(ThirdParty/SDL EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# OpenGL
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
endif ()

# Windows specific
if (WIN32)
    # Add a post-build command to copy the SDL DLL to the output directory
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}"
            -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            VERBATIM
    )

endif ()