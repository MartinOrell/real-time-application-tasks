## helloSFML
This is a project to showcase using SFML for graphics in a code realTime application. SFML is used by a user library called *gui*. gui is then used by a capsule.

### Building gui
Before using gui which contains SFML, the library needs to be built. Use a terminal inside the gui folder to first generate the makefiles and then build the library
```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build/
```
### building project
After the gui library is built, then you can compile the project as normal with the .tcjs file.

### misc
#### including libraries
This is the content inside the .tcjs file specifically used to be able to run SFML.

```
tc.userLibraries = [
    "../../gui/build/libgui.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-audio.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-graphics.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-main.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-network.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-system.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-window.a"
];
tc.inclusionPaths = [
    "../../gui/include",
    "../../gui/build/_deps/sfml-src/include"
]
```

#### Copying files to target
In order for SFML to work some dll files needs to be in the same folder as the executable. These files can be found at gui/build/bin. Those files are copied automatically when building. The same is true for the res folder which contains the font for the print. The copy instruction is defined in the TC
```
tc.makeCommand = 'cp -R -Force ../../gui/build/bin/*  .; make; cp -R -Force ../../res .';
```