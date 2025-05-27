project(PokeRemakeGame)
set(CMAKE_CXX_STANDARD 20)

include_directories(../Runtime)

set(SOURCES
        TestGame.cpp
)
add_library(${PROJECT_NAME} ${SOURCES})
