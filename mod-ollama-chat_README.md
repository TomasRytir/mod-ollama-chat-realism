# Installing and Building the mod-ollama-chat Module for AzerothCore (Windows 11 + vcpkg + Visual Studio)

This guide describes the complete process of configuring, compiling, and integrating the `mod-ollama-chat` module into the AzerothCore server on **Windows 11** using **vcpkg** and **Visual Studio**.

---

## 🛠️ Installing and Configuring vcpkg

1. Clone the vcpkg repository to `C:\vcpkg`
2. In PowerShell, run:

```powershell
.ootstrap-vcpkg.bat
```

If the installation fails due to missing 7-Zip:

- Download `7z2409.7z`
- Extract it manually (e.g., using WinRAR)
- Ensure the `C:\vcpkg\downloads\` folder contains the extracted files

---

## 📦 Installing libraries via vcpkg

```powershell
.cpkg.exe install curl nlohmann-json
```

You should see something like:

```
Found nlohmann_json: C:/vcpkg/installed/x64-windows/share/nlohmann_json/nlohmann_jsonConfig.cmake
```

---

## 🔧 Fixing `mod-ollama-chat.cmake`

**Path:** `C:\AzerothCore\modules\mod-ollama-chat\mod-ollama-chat.cmake`

**Replace incorrect line:**

```cmake
target_link_libraries(mod-ollama-chat
```

**With:**

```cmake
if(TARGET modules)
target_link_libraries(modules PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
)
endif()

find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
```

---

## 🧱 Creating `CMakeSettings.json` for Visual Studio

**Path:** `C:\AzerothCore\CMakeSettings.json`

```json
[
  {
    "name": "x64-Release",
    "generator": "Visual Studio 17 2022",
    "configurationType": "RelWithDebInfo",
    "buildRoot": "${projectDir}\out\build\${name}",
    "installRoot": "${projectDir}\out\install\${name}",
    "cmakeCommandArgs": "-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake",
    "variables": [
      {
        "name": "CMAKE_BUILD_TYPE",
        "value": "Release"
      }
    ]
  }
]
```

⚠️ Ensure it has the `.json` extension!

---

## ⚙️ Manual configuration via CMake (Alternative)

```bash
cd C:\AzerothCore
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

---

## 🎯 Opening the project in Visual Studio

- Open `C:\AzerothCore\build\AzerothCore.sln`
- Set:
  - **Configuration**: `RelWithDebInfo`
  - **Platform**: `x64`

In Solution Explorer:
- Right-click on `ALL_BUILD` → **Build**

---

## ✅ After build completion

Executables:
```
C:\AzerothCore\build\bin\RelWithDebInfo\worldserver.exe
C:\AzerothCore\build\bin\RelWithDebInfo\authserver.exe
```

Config files:
```
C:\AzerothCore\etc\worldserver.conf.dist
C:\AzerothCore\etc\authserver.conf.dist
```

Rename them:
```
worldserver.conf
authserver.conf
```

---

## 🧩 Summary of Solved Issues

- ✅ Missing curl.h and json.hpp – fixed via vcpkg
- ✅ 7-Zip extraction – solved manually
- ✅ Wrong bootstrap folder – corrected
- ✅ Could NOT find CURL – solved via CMakeSettings
- ✅ Incorrect target_link_libraries – fixed
- ✅ Missing `.json` extension – renamed
- ✅ Successful Visual Studio build confirmed by output logs

---

This guide helps ensure smooth building of `mod-ollama-chat` with Visual Studio + vcpkg on Windows 11.
