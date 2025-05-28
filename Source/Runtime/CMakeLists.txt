project(PokeRemakeRuntime)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        Core/Delegate.cpp
        Engine/Game.cpp
        Core/Logging.h
        Engine/Actors/Actor.cpp
        Engine/Actors/Character.cpp
        Engine/Actors/Grid.cpp
        Engine/Engine.cpp
        Engine/InputManager.cpp
        Engine/Object.cpp
        Engine/World.cpp
        Platform/Generic/IPlatform.cpp
        Platform/SDL/SDLPlatform.cpp
        Renderer/Generic/IRenderer.cpp
        Renderer/IDrawable.h
        Renderer/SDL/SDLRenderer.cpp
        Core/Vector.h
        Core/Matrix.h
)
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
include_directories(
        ${PROJECT_SOURCE_DIR}
        ../../ThirdParty
        ../../ThirdParty/SDL/include
)
