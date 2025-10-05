# Nazwa kompilatora
CC = C:/msys64/ucrt64/bin/gcc.exe

# Nazwa pliku wykonywalnego
TARGET = build/main.exe

# Flagi dla kompilatora (debugowanie, ostrzeżenia)
CFLAGS = -g -Wall

# Automatycznie znajdź wszystkie pliki .c w folderze src i podfolderach
SRCS = $(shell find src -name "*.c")

# Ścieżki dołączania (include)
INCLUDES = -Iinclude \
           -IC:/msys64/ucrt64/include \
           -IC:/msys64/ucrt64/include/SDL3 \
           -Isrc/engine \
           -Isrc/graphics \
           -Isrc/utils

# Ścieżki do bibliotek i same biblioteki
LIBS = -LC:/msys64/ucrt64/lib \
       -lSDL3 \
       -lopengl32 \
       -lshlwapi

# Główna reguła, która zostanie uruchomiona po wpisaniu 'make'
all: $(TARGET)

# Reguła opisująca, jak stworzyć plik wykonywalny $(TARGET)
$(TARGET):
	@echo "Building project..."
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(INCLUDES) $(LIBS)
	@echo "Build finished!"

# Reguła do czyszczenia skompilowanych plików
clean:
	@echo "Cleaning build files..."
	rm -f $(TARGET)