CC = C:/msys64/ucrt64/bin/gcc.exe
TARGET = build/main.exe
ASSET_INDEXER = build/asset_indexer.exe
CFLAGS = -g -Wall

INCLUDES = -I. -Isrc -Iinclude -Ilibs -IC:/msys64/ucrt64/include -IC:/msys64/ucrt64/include/SDL3 -Isrc/engine -Isrc/graphics -Isrc/utils
LIBS = -LC:/msys64/ucrt64/lib -lSDL3 -lopengl32 -lshlwapi

# Znajdź wszystkie pliki .c OPRÓCZ build_tools
SOURCES = $(filter-out src/build_tools/%, $(wildcard src/*.c src/*/*.c))

# Kompiluj asset_indexer, uruchom go, potem kompiluj main
all: $(ASSET_INDEXER) run-indexer $(TARGET)

# Kompiluj asset_indexer z cJSON (zależy od plików źródłowych)
$(ASSET_INDEXER): src/build_tools/asset_indexer.c libs/cJSON.c libs/cJSON.h
	$(CC) $(CFLAGS) src/build_tools/asset_indexer.c libs/cJSON.c -o $(ASSET_INDEXER) $(INCLUDES)

# Uruchom asset_indexer (zawsze uruchamiaj, nawet jeśli .exe się nie zmienił)
run-indexer: $(ASSET_INDEXER)
	$(ASSET_INDEXER)

# Kompiluj main z cJSON i hashmap (zależy od wszystkich plików źródłowych)
$(TARGET): $(SOURCES) libs/cJSON.c libs/cJSON.h libs/hashmap.c libs/hashmap.h
	$(CC) $(CFLAGS) $(SOURCES) libs/cJSON.c libs/hashmap.c -o $(TARGET) $(INCLUDES) $(LIBS)

clean:
	del /Q build\main.exe build\asset_indexer.exe 2>nul || exit 0

.PHONY: all run-indexer clean