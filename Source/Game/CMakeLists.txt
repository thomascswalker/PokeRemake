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
        BattleMode.h
        MapMode.cpp
        BattleMode.cpp
        BattleView.cpp
        BattleView.h
        Interface/TransitionOverlay.h
        FadeTransition.h
        Pokedex.h
        Pokedex.cpp
        Party.h
)
add_library(${PROJECT_NAME} ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
