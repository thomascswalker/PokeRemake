cmake_minimum_required(VERSION 3.31)
project(PokeRemake)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Game)
add_subdirectory(Runtime)

add_executable(${PROJECT_NAME} Game/Main.cpp)
target_link_libraries(
        ${PROJECT_NAME} PokeRemakeRuntime
)
target_link_libraries(
        ${PROJECT_NAME} PokeRemakeGame
)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

