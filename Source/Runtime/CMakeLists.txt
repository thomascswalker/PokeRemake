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
        Engine/Actors/Chunk.cpp
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
        Engine/Actors/Tile.h
        Engine/Sprite.h
        Engine/Components/CharacterMovementComponent.h
        Engine/Components/CharacterMovementComponent.cpp
        Engine/Actors/Tile.cpp
        Interface/Widget.h
        Interface/Button.h
        Interface/Spinner.h
        Interface/Canvas.h
        Interface/Widget.cpp
        Interface/Layout.h
        Interface/Text.h
)
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

# Copied here because it doesn't propagate from the main CMakeLists.txt
if (IN_EDITOR)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _EDITOR=1)
else ()
    target_compile_definitions(${PROJECT_NAME} PUBLIC _EDITOR=0)
endif ()