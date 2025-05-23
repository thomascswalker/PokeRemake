project(PokeRemakeRuntime)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        Platform/Generic/IPlatform.cpp
        Renderer/Generic/IRenderer.cpp
        Platform/SDL/SDLPlatform.cpp
        Platform/SDL/SDLPlatform.h
        Engine/Engine.cpp
        Engine/Engine.h
        Engine/InputManager.cpp
        Engine/InputManager.h
)
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
include_directories(../../ThirdParty)
include_directories(../../ThirdParty/SDL/include)