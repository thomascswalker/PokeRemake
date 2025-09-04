project(RuntimeTests)
set(CMAKE_CXX_STANDARD 23)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ../Source/Runtime
        ../ThirdParty
        ../ThirdParty/glm
        ../ThirdParty/SDL/include
        ../ThirdParty/JSON/include/
        ../ThirdParty/nativefiledialog-extended/include
)

add_executable(${PROJECT_NAME}
        #        VectorTests.cpp
        #        MatrixTests.cpp
        #        ColorTests.cpp
        #        ArrayTests.cpp
        ParamBlockTests.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PokeRemakeRuntime)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)