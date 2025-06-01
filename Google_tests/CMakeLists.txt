project(RuntimeTests)
include_directories(${gtest_SOURCE_DIR})

add_executable(${PROJECT_NAME}
        VectorTests.cpp
        MatrixTests.cpp
)

target_link_libraries(${PROJECT_NAME} PokeRemakeRuntime)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)