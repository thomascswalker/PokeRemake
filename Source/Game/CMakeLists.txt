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
        Core/PokeParty.cpp
        Core/PokeStorage.cpp
        Core/Pokedex.cpp
        Core/Pokemon.h
        Interface/BattleHUD.h
        Interface/GameHUD.cpp
        Interface/TransitionOverlay.h
        MainGame.cpp
        Battle/BattleMode.cpp
        Modes/MapMode.cpp
        Battle/BattleManager.cpp
        Core/GameConstants.h
        Interface/BattleHUD.cpp
        Interface/BattleActionMenu.cpp
        Interface/DialogBox.cpp
        Core/Font.cpp
        Actors/Trainer.cpp
        Core/GameSerializer.h
        Interface/Frame.cpp
        Interface/BattleMoveMenu.cpp
        Interface/BattleMoveMenu.h
)
add_library(${PROJECT_NAME} ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
