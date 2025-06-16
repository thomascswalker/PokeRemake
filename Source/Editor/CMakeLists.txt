project(PokeRemakeEditor)
set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ../Runtime
        ../../ThirdParty
        ../../ThirdParty/SDL/include
        ../../ThirdParty/json/include/
        ../../ThirdParty/nativefiledialog-extended/include
)

set(SOURCES
        EditorGame.cpp
        EditorView.cpp
)
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

# Copied here because it doesn't propagate from the  main CMakeLists.txt
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
if (IN_EDITOR)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _EDITOR=1)
else ()
    target_compile_definitions(${PROJECT_NAME} PUBLIC _EDITOR=0)
endif ()