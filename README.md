# PokeRemake

This is a personal project to attempt to recreate Pokemon Generation 1 in modern C++.

- [Examples](#examples)
- [Building from Source](#building-from-source)
- [Roadmap](#roadmap)

## Examples

<details open>
<summary>Expand</summary>

### Gameplay
![](https://github.com/thomascswalker/PokeRemake/blob/1de26cb813598a3aeabb21c641f6eb65a9ce9205/Images/example.gif)

### Editor
![](https://github.com/thomascswalker/PokeRemake/blob/4bf07c20b49066302c21cde95010013bba877e9f/Images/Editor.png)

</details>

## Building from Source

### Prerequisites

- CMake 3.31+
- C++23
- MinGW (preferred)
- Ninja (preferred)

### Steps

1. Clone this repository.
2. `cd` into the repository directory.
3. Run `cmake.exe -DCMAKE_BUILD_TYPE=<Debug|Release> "-DCMAKE_MAKE_PROGRAM=ninja.exe" -DIN_EDITOR=<OFF|ON> -G Ninja -S . -B .\cmake-build-<debug|release>`.

### CMAKE Arguments

#### Native
- `-DCMAKE_BUILD_TYPE` = `Debug` or `Release`
- `-DCMAKE_MAKE_PROGRAM` = `ninja.exe` or your preferred build system.

#### Custom
- `-DIN_EDITOR` = `OFF` for building a standalone game, `ON` for building the editor.

## Roadmap

### Core

#### Editor
- [x] Editor mode

#### Save/Load
- [x] Save/load maps from editor
- [x] Load/Unload maps into game
- [ ] Load/Unload adjacent maps

### Rendering
- [x] Basic renderer

#### Animation
- [x] Character movement
- [ ] Decor (grass, water)

### Interface
- [x] User interface

### Gameplay
- [x] Player movement

#### Battle
- [ ] Battle mode

#### Menu
- [ ] Open menu

#### Interaction
- [ ] Characters
- [ ] Environment


