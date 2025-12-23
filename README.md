# ReiToEngine

## Features

* **Temp Test Project**: Now can type in `n` to create new window and `Esc` in main window to exit.

## Build & Run

This project uses **CMake Presets** to simplify the build process.

### Prerequisites

* **CMake** (3.20+)
* **[Vulkan SDK](https://vulkan.lunarg.com/)**
* **Compiler**: Clang (macOS) or MSVC/LLVM (Windows)
* **Ninja** (Recommended)

### 1. Pull (Clone)

```bash
git clone https://github.com/tolly201/ReiToEngine.git
cd ReiToEngine
git submodule update --init --recursive
```
Submodule contains Assimp, mimalloc and stb.

### 2. Configure

List available presets:
```bash
cmake --list-presets
```

Configure using a preset (e.g., `macos-clang-debug` for macOS or `windows-msvc-x64` for Windows):
```bash
cmake --preset macos-clang-debug
```
> **Note**: If you encounter errors related to compiler or linker paths, please verify the toolchain configuration files in `./Engine/Scripts/cmake_toolchain/` and adjust them to match your environment.

### 3. Build

```bash
cmake --build --preset macos-clang-debug
```

### 4. Run

**macOS:**
```bash
# Run the Editor
./builds/macos-clang-debug/ReiToEngine/bin/ReiToEditor.app/Contents/MacOS/ReiToEditor
```

**Windows:**
```powershell
# Run the Editor
.\builds\windows-llvm-x64\ReiToEngine\bin\ReiToEditor.exe
```

## Credits

Links of repos or other resources used by this project:

* [1] [Kohi Engine](https://github.com/travisvroman/kohi)
* [2] [tinyrenderer](https://github.com/ssloy/tinyrenderer)
* [3] [stb](https://github.com/nothings/stb.git)
