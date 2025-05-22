project(PokeRemakeGame)
set(CMAKE_CXX_STANDARD 20)

include_directories(../Runtime)
include_directories(../../ThirdParty/SDL/include)

add_library(${PROJECT_NAME} Main.cpp)

