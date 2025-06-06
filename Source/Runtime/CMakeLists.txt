project(PokeRemakeRuntime)
set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ../../ThirdParty
        ../../ThirdParty/glm
        ../../ThirdParty/SDL/include
)

set(SOURCES
        Application/Application.cpp
        Core/Delegate.cpp
        Core/Logging.h
        Core/Matrix.h
        Core/Vector.h
        Engine/Actors/Actor.cpp
        Engine/Actors/Character.cpp
        Engine/Actors/Grid.cpp
        Engine/CameraView.h
        Engine/Engine.cpp
        Engine/Game.cpp
        Engine/InputManager.cpp
        Engine/Object.cpp
        Engine/World.cpp
        Renderer/IDrawable.h
        Renderer/Renderer.cpp
        Renderer/IRHI.h
        Application/ArgParser.h
        Application/Context.h
        Renderer/Shader.cpp
        Engine/Components/Component.cpp
        Engine/Components/CameraComponent.cpp
        Engine/Actors/Dummy.h
        Engine/Actors/PlayerCharacter.cpp
        Engine/Texture.cpp
        Core/Files.h
        Engine/Mesh.h
        Core/Constants.h
        Core/Settings.h
        Core/Settings.cpp
        Engine/Chunk.h
        Engine/Sprite.h
)
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
