project(PokeRemakeRuntime)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        Platform/Generic/Platform.cpp
        Platform/Windows/WindowsPlatform.cpp
        Platform/Windows/WindowsPlatform.h
)
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
