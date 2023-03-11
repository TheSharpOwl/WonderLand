# WonderLand
Some 2D game (name subject to change)

## Demos for stages of development
[Youtube list of versions history Link](https://www.youtube.com/playlist?list=PLgAAHVNxxjaW3VWccW8dsZ5yQXRceOENX)
### Latest stage so far:
[![Latest Development stage](https://img.youtube.com/vi/EY9d98auyXM/0.jpg)](https://www.youtube.com/watch?v=EY9d98auyXM&list=PLgAAHVNxxjaW3VWccW8dsZ5yQXRceOENX&index=3)
## Dependencies:
For now writing what I am using (will update it to all possible versions..etc)
* Visual Studio 2022
* CMake 3.21
* SFML (used 2.5.1) (download same version as mine from [this link in offical website](https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip))
* irrklang (used 1.6 x64) (download my version from [this link in offical website](https://www.ambiera.at/downloads/irrKlang-64bit-1.6.0.zip))

## How to Run:
1. Put SFML and irrklang in the same folder of this repo in new folder `modules`(optional)
2. Example:
    ```bash
    git clone https://github.com/TheSharpOwl/WonderLand.git
    cd Wonderland
    mkdir build
    cd build
    cmake .. -A x64 -G "Visual Studio 17 2022" -DSFML_PATH="C:/Wonderland/modules/SFML-2.5.1" -DSFML_PATH="C:/Wonderland/modules/irrKlang-64bit-1.6.0"
    ```

    **Note that relative path to the libraries might not work so try to pass absolute path**

3. Open the `.sln` file in `Wonderland/build` and build it inside Visual Studio (just build or try to run and close the error window)
4. Copy all dll files for all libraries in the modules folders (from their bin folders and make sure you copy the right ones for debug or release) to the built exe path
5. Run inside visual studio
6. Have fun ! (I guess ?)