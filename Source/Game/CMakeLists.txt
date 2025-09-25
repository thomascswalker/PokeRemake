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
        BattleView.cpp
        Core/PokeParty.cpp
        Core/PokeStorage.cpp
        Core/Pokedex.cpp
        Core/Pokemon.h
        FadeTransition.h
        Interface/BattleHUD.h
        Interface/GameHUD.cpp
        Interface/TransitionOverlay.h
        MainGame.cpp
        Modes/BattleMode.cpp
        Modes/MapMode.cpp
)
add_library(${PROJECT_NAME} ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
