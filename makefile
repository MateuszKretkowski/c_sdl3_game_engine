CC = C:/msys64/ucrt64/bin/gcc.exe
TARGET = build/main.exe
ASSET_INDEXER = build/asset_indexer.exe
COMPONENT_GENERATOR = build/component_header_generator.exe
CFLAGS = -g -Wall

INCLUDES = -I. -Isrc -Iinclude -Ilibs -IC:/msys64/ucrt64/include -IC:/msys64/ucrt64/include/SDL3 -Isrc/engine -Isrc/graphics -Isrc/utils
LIBS = -LC:/msys64/ucrt64/lib -lSDL3 -lopengl32 -lshlwapi

SOURCES = $(filter-out src/build_tools/%, $(wildcard src/*.c src/*/*.c))

all: $(COMPONENT_GENERATOR) run-component-generator $(ASSET_INDEXER) run-indexer $(TARGET)

$(COMPONENT_GENERATOR): src/build_tools/component_header_generator.c
	$(CC) $(CFLAGS) src/build_tools/component_header_generator.c -o $(COMPONENT_GENERATOR) $(INCLUDES)

run-component-generator: $(COMPONENT_GENERATOR)
	$(COMPONENT_GENERATOR)

$(ASSET_INDEXER): src/build_tools/asset_indexer.c libs/cJSON.c libs/cJSON.h
	$(CC) $(CFLAGS) src/build_tools/asset_indexer.c libs/cJSON.c -o $(ASSET_INDEXER) $(INCLUDES)

run-indexer: $(ASSET_INDEXER)
	$(ASSET_INDEXER)

# Kompiluj main z cJSON i hashmap (zależy od wszystkich plików źródłowych)
$(TARGET): $(SOURCES) libs/cJSON.c libs/cJSON.h libs/hashmap.c libs/hashmap.h
	$(CC) $(CFLAGS) $(SOURCES) libs/cJSON.c libs/hashmap.c -o $(TARGET) $(INCLUDES) $(LIBS)

clean:
	del /Q build\main.exe build\asset_indexer.exe build\component_header_generator.exe 2>nul || exit 0

.PHONY: all run-component-generator run-indexer clean