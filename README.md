# WonderLand
Some 2D game (name subject to change)

## Dependencies:
For now writing what I am using (will update it to all possible versions..etc)
* Visual Studio 2022
* CMake 3.21
* SFML (download same version as mine from [this link in offical website](https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip))


## How to Run:
1. Put SFML in the same folder of this repo in new folder `modules`(optional)
2. Example (in case sfml 2.5.1 is used and step 1 is done):
    ```bash
    git clone 
    cd Wonderland
    git submodule update --init --recursive
    mkdir build
    cd build
    cmake .. -A x64 -G "Visual Studio 17 2022" -DSFML_PATH="../modules/SFML-2.5.1"
    ```

3. Open the `.sln` file in `Wonderland/build` and build it inside Visual Studio (just build or try to run and close the error window)
4. Copy all dll files in SFML bin folder to the built exe path
5. Run inside visual studio
