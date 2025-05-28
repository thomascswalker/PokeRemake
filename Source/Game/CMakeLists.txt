project(PokeRemakeGame)
set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ../Runtime
        ../../ThirdParty
        ../../ThirdParty/SDL/include
)

set(SOURCES
        TestGame.cpp
)
add_library(${PROJECT_NAME} ${SOURCES})
