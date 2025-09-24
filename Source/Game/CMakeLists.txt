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
        MainGame.cpp
        Modes/BattleMode.h
        Modes/MapMode.cpp
        Modes/BattleMode.cpp
        BattleView.cpp
        BattleView.h
        Interface/TransitionOverlay.h
        FadeTransition.h
        Core/Pokedex.cpp
        Core/PokeParty.h
        Core/PokeStorage.h
        Core/Pokemon.h
        Core/PokeParty.cpp
        Core/PokeStorage.cpp
)
add_library(${PROJECT_NAME} ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
