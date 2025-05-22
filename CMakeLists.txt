cmake_minimum_required(VERSION 3.31)
project(PokeRemake)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Source/Game)
add_subdirectory(Source/Runtime)

add_executable(${PROJECT_NAME} Source/Game/Main.cpp)
target_link_libraries(
        ${PROJECT_NAME} PUBLIC PokeRemakeRuntime
)
target_link_libraries(
        ${PROJECT_NAME} PUBLIC PokeRemakeGame
)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
set (CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
message (STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
add_subdirectory(ThirdParty/SDL EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)