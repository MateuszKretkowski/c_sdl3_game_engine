# Game Engine, written in C.

## Build (Windows)

Silnik używa SDL3 + OpenGL 3.3 Core i buduje się przez `make` z toolchainem MSYS2 UCRT64. Żadna ścieżka nie jest już zaszyta na sztywno w kodzie/konfiguracji — wystarczy mieć poniższe narzędzia na PATH.

1. Zainstaluj [MSYS2](https://www.msys2.org/) (polecana domyślna lokalizacja `C:\msys64` — jej dotyczą ścieżki w `.vscode/*.json`; jeśli zainstalujesz gdzie indziej, popraw `compilerPath`/`miDebuggerPath`/task w `.vscode`).
2. Otwórz terminal **MSYS2 UCRT64** (z Menu Start) i zainstaluj toolchain + SDL3:
   ```
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-SDL3
   ```
3. Dodaj `C:\msys64\ucrt64\bin` do zmiennej środowiskowej `PATH` (Windows: *Edytuj zmienne środowiskowe konta*). Ten jeden katalog daje `gcc`, `gdb`, `mingw32-make` **i** `SDL3.dll` (potrzebny, żeby `main.exe` w ogóle się uruchomił).
4. W katalogu projektu:
   ```
   mingw32-make
   ```
   (w VS Code: Ctrl+Shift+B, albo F5 żeby zbudować i od razu debugować)
5. Uruchom `build\main.exe`.

Uwaga: `mingw32-make` (z `ucrt64\bin`), nie `make` z `usr\bin` — to drugie to build MSYS-owy i w parze z natywnym gcc z `ucrt64` potrafi się wykrzaczać (np. błędami o tworzeniu plików tymczasowych).

macOS/Linux nie są obecnie wspierane (build i `src/engine/init.h` zakładają Windows albo Linux-owe `/proc/self/exe` — macOS wymaga osobnej gałęzi).
