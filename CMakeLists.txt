cmake_minimum_required(VERSION 3.31)

message("CMAKE Version: ${CMAKE_VERSION}")

project(PokeRemake)
set(CMAKE_CXX_STANDARD 20)

option(IN_EDITOR "IN_EDITOR" OFF)

include(FetchContent)

add_subdirectory(Source/Runtime)
if (IN_EDITOR)
    add_subdirectory(Source/Editor)
else ()
    add_subdirectory(Source/Game)
endif ()

# Third Party Libraries

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
include_directories(
        ThirdParty
        ThirdParty/glm
        ThirdParty/SDL/include
)

# Build the respective executable either for the editor or the game.

if (IN_EDITOR)
    message(STATUS "Building Editor")
    add_executable(${PROJECT_NAME} Source/Editor/Main.cpp)
    target_link_libraries(${PROJECT_NAME} PUBLIC PokeRemakeEditor)
else ()
    message(STATUS "Building Game")
    add_executable(${PROJECT_NAME} Source/Game/Main.cpp)
    target_link_libraries(${PROJECT_NAME} PUBLIC PokeRemakeGame)
endif ()


target_link_libraries(
        ${PROJECT_NAME} PUBLIC PokeRemakeRuntime
)

# Set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../$<CONFIGURATION>")

# SDL3
add_subdirectory(ThirdParty/SDL EXCLUDE_FROM_ALL)
#add_subdirectory(ThirdParty/SDL_image EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# GLM
add_subdirectory(ThirdParty/glm EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

# Vulkan
find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

# JSON

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(${PROJECT_NAME} PUBLIC ThirdParty/json/include)

# Native file dialog

add_subdirectory(ThirdParty/nativefiledialog-extended)
target_link_libraries(${PROJECT_NAME} PRIVATE nfd)
target_include_directories(${PROJECT_NAME} PUBLIC ThirdParty/nativefiledialog-extended/include)

# Windows specific
if (WIN32)
    # Copy output SDL .dll
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}"
            -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            VERBATIM
    )
    #    add_custom_command(
    #            TARGET ${PROJECT_NAME} POST_BUILD
    #            COMMAND "${CMAKE_COMMAND}"
    #            -E copy_if_different
    #            "$<TARGET_FILE:SDL3_image::SDL3_image>"
    #            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    #            VERBATIM
    #    )
    # D3D12
    list(APPEND CMAKE_PREFIX_PATH "C:/Windows/System32")
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d12.lib)
endif ()

# Copy shaders to the output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy_directory
        "${CMAKE_SOURCE_DIR}/Resources/Shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

# Tests
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest)
add_subdirectory(Google_tests)

