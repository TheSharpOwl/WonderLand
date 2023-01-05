# WonderLand
Some 2D game (name subject to change)

## Dependencies:
For now writing what I am using (will update it to all possible versions..etc)
* Visual Studio 2022
* CMake 3.21
* SFML (will be downloaded with submodules)


## How to Run:
```bash
git clone 
cd Wonderland
git submodule update --init --recursive
mkdir build
cd build
cmake .. -A x64 -G "Visual Studio 17 2022"
```

Then open the `.sln` file in `Wonderland/build` and build it inside Visual Studio