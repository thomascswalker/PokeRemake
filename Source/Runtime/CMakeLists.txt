project(PokeRemakeRuntime)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        Platform/Generic/IPlatform.cpp
        Renderer/Generic/IRenderer.cpp
        Platform/SDL/SDLPlatform.cpp
        Engine/Engine.cpp
        Engine/InputManager.cpp
        Core/Delegate.cpp
        Core/Logging.h
        Engine/Actors/World.cpp
        Engine/Actors/World.h
        Engine/Actors/Actor.cpp
        Engine/Actors/Actor.h
        Renderer/IDrawable.h
        Renderer/SDL/SDLRenderer.h
        Renderer/SDL/SDLRenderer.cpp
        Engine/Actors/Object.cpp
        Engine/Actors/Object.h
)
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
include_directories(
        ${PROJECT_SOURCE_DIR}
        ../../ThirdParty
        ../../ThirdParty/SDL/include
)
