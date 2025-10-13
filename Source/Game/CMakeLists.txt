project(PokeRemakeGame)
set(CMAKE_CXX_STANDARD 23)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ../Runtime
        ../../ThirdParty
        ../../ThirdParty/SDL/include
        ../../ThirdParty/JSON/include/
        ../../ThirdParty/nativefiledialog-extended/include
)

set(SOURCES
        Actors/Trainer.cpp
        Battle/BattleManager.cpp
        Battle/BattleMode.cpp
        Core/GameConstants.h
        Core/GameSerializer.h
        Core/PokeParty.cpp
        Core/PokeStorage.cpp
        Core/Pokedex.cpp
        Core/Pokemon.h
        Core/TextRenderer.cpp
        Interface/Battle/BattleActionMenu.cpp
        Interface/Battle/BattleHUD.cpp
        Interface/Battle/BattleHUD.h
        Interface/Battle/BattleMoveMenu.cpp
        Interface/DialogBox.cpp
        Interface/Frame.cpp
        Interface/GameHUD.cpp
        Interface/TransitionOverlay.h
        MainGame.cpp
        Modes/MapMode.cpp
        Interface/Battle/BattlePPMenu.cpp
        Interface/Battle/BattlePPMenu.h
        Core/Ids.h
        Interface/Battle/BattleInterfaceConstants.h
)
add_library(${PROJECT_NAME} ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
