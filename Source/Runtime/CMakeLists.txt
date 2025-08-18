project(PokeRemakeRuntime)
set(CMAKE_CXX_STANDARD 23)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ../../ThirdParty
        ../../ThirdParty/glm
        ../../ThirdParty/SDL/include
        ../../ThirdParty/JSON/include/
        ../../ThirdParty/nativefiledialog-extended/include
)

set(SOURCES
        Application/Application.cpp
        Application/ArgParser.h
        Application/Context.h
        Core/Bitmask.h
        Core/CFwd.h
        Core/Color.cpp
        Core/Common.h
        Core/Constants.h
        Core/Containers.h
        Core/CoreFwd.h
        Core/Decl.h
        Core/Delegate.cpp
        Core/Files.h
        Core/Json.h
        Core/Logging.h
        Core/Macros.h
        Core/Matrix.h
        Core/Meta.cpp
        Core/Time.h
        Core/Vector.h
        Engine/Actors/Actor.cpp
        Engine/Actors/Character.cpp
        Engine/Actors/Collider.h
        Engine/Actors/Interactable.h
        Engine/Actors/Map.cpp
        Engine/Actors/PlayerCharacter.cpp
        Engine/Actors/Portal.cpp
        Engine/Actors/Selectable.h
        Engine/Actors/SceneryActor.cpp
        Engine/CameraView.h
        Engine/Components/CameraComponent.cpp
        Engine/Components/CharacterMovementComponent.cpp
        Engine/Components/CollisionComponent.cpp
        Engine/Components/Component.cpp
        Engine/Engine.cpp
        Engine/Game.cpp
        Engine/Input.cpp
        Engine/InputContext.cpp
        Engine/MapManager.cpp
        Engine/Object.cpp
        Engine/Serialization.cpp
        Engine/Settings.h
        Engine/Sprite.h
        Engine/Texture.cpp
        Engine/Tileset.cpp
        Engine/World.cpp
        Interface/AbstractButton.cpp
        Interface/Box.h
        Interface/Button.h
        Interface/ButtonGroup.h
        Interface/Dropdown.cpp
        Interface/GridView.h
        Interface/Group.h
        Interface/HUD.h
        Interface/Image.h
        Interface/ItemView.h
        Interface/Layout.h
        Interface/Panel.h
        Interface/ScrollArea.h
        Interface/Spacer.h
        Interface/Spinner.h
        Interface/Text.h
        Interface/Widget.cpp
        Interface/Widget.h
        Renderer/Drawable.h
        Renderer/Renderer.cpp
        Interface/Game/GameHUD.cpp
        Interface/Game/DialogBox.h
        Engine/Components/SpriteComponent.cpp
        Engine/Components/SpriteComponent.h
        Core/String.h
        Core/Array.h
        Core/Json.cpp
        Engine/Components/InteractionComponent.h
        Engine/Components/InteractionComponent.cpp
        Engine/Components/SelectionComponent.h
        Engine/Components/SelectionComponent.cpp
)
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

# Copied here because it doesn't propagate from the main CMakeLists.txt
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
if (IN_EDITOR)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _EDITOR=1)
else ()
    target_compile_definitions(${PROJECT_NAME} PUBLIC _EDITOR=0)
endif ()