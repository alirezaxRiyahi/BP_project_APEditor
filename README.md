# AP Editor - C++ Code Editor

**Author:** Alireza Riyahi

AP Editor is a C++ code editor with a graphical user interface built using **SDL2**.  
It provides various features for writing, compiling, and running C++ code directly inside the editor.

---

## ✨ Features
- Syntax highlighting for C++ keywords
- 🎨 Light & Dark themes
- 📁 Project management (create, save, open)
- 🔧 Compile and run directly from the editor
- 🐛 Error detection (syntax and libraries)
- 📋 Text editing (copy, paste, delete, select)
- 🔍 Jump to specific line
- 🌳 Project tree view

---

## 📂 Folder Structure
```plaintext
C:.
│   CMakeLists.txt
│   README.md
│
├───.idea
├───assets
│       consola.ttf
│       icon.ico
│       New Recording.mp3
│       photo16646248983.jpg
│
├───data
│   ├───log
│   │       text_handling.txt
│   │
│   └───Tree_project
│
├───include
└───src
        main.cpp
        resource.rc
```
       

## ⚙️ Requirements

- SDL2

- SDL2_image

- SDL2_ttf

- SDL2_mixer

- SDL2_gfx

- GCC / G++

## 💻 Installation (Windows with MSYS2)
```
Install MSYS2

Install required libraries:

pacman -S mingw-w64-x86_64-SDL2
pacman -S mingw-w64-x86_64-SDL2_image
pacman -S mingw-w64-x86_64-SDL2_ttf
pacman -S mingw-w64-x86_64-SDL2_mixer
pacman -S mingw-w64-x86_64-SDL2_gfx
pacman -S mingw-w64-x86_64-gcc
```

## 🚀 Build & Run
```
Using CMake
mkdir build
cd build
cmake ..
make
./AP_Editor

Manual build
g++ -std=c++11 src/main.cpp -o AP_Editor -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx
```
## 📝 Usage
File Menu

New Project → Create new project

Save Project → Save current project

Save As → Save with a new name

Exit → Quit editor

Edit Menu

Undo / Redo

View Menu

Mode → Switch between Light/Dark theme

Toolbar

Save → Save project

Compile → Compile code

Run → Run executable

## 🐞 Error Detection

Missing libraries (e.g., iostream, cmath)

Brackets, braces, parentheses

Missing semicolons

Comments

Operators

Strings

Invalid variable names

## 🔧 Technical Notes

Regex-based syntax analysis

Undo/redo up to 10 steps

Scroll support for large files

Embedded terminal for compiler output

## 👨‍💻 Development

Built with C++11 and SDL2

Uses design patterns for clean architecture

Supports advanced text editing features
