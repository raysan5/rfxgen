# rFXGen CMake

This allows building [rFXGen](https://raylibtech.itch.io/rfxgen) with [CMake](https://cmake.org).

## Usage

To compile the project, use one of the following dependending on your build target...

### Desktop (Windows, Linux, macOS)

Use the following to build for desktop:

``` bash
cmake -B build
cmake --build build
```

### Web (HTML5 - emscripten)

Compiling for the web requires the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html):

``` bash
mkdir build
cd build
emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html"
emmake make
```
