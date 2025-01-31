## Usage

- `Esc`: Quit
- `Mouse Scroll`: Move towards the video
- `Mouse`: View

## Windows build
To build this on windows you'll need
1. Cmake
2. Any c++ compiler

```bash
git clone --recursive https://github.com/LastLosts/CurvedVideoRenderer
cd CurvedVideoRenderer
cmake -S . -B build-windows/
cmake --build build-windows/
```

## Unix build

To build this on Unix you'll need
1. Cmake
2. Any c++ compiler
3. PkgConfig
4. Ffmpeg
```bash
git clone --recursive https://github.com/LastLosts/CurvedVideoRenderer
cd CurvedVideoRenderer
cmake -S . -B build-linux/
cmake --build build-linux/
```
