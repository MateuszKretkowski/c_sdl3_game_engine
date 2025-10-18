#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

const char *components_path = "src/components/";
const char *output_path = "src/components/components.h";
const char *header_extension = ".h";

int main() {
    struct dirent *entry;
    DIR *dir = opendir(components_path);

    if (!dir) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("ERROR: Cannot open '%s'\n", components_path);
        printf("Current directory: %s\n", cwd);
        return 1;
    }

    FILE *output = fopen(output_path, "w");
    if (!output) {
        printf("ERROR: Cannot create output file: %s\n", output_path);
        closedir(dir);
        return 1;
    }

    fprintf(output, "#ifndef COMPONENTS_H\n");
    fprintf(output, "#define COMPONENTS_H\n\n");

    printf("Generating components.h...\n");

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        size_t name_len = strlen(entry->d_name);
        size_t ext_len = strlen(header_extension);

        if (name_len > ext_len &&
            strcmp(entry->d_name + name_len - ext_len, header_extension) == 0) {

            if (strcmp(entry->d_name, "components.h") == 0) {
                continue;
            }

            const char *component_suffix = "_component.h";
            size_t suffix_len = strlen(component_suffix);

            if (name_len >= suffix_len &&
                strcmp(entry->d_name + name_len - suffix_len, component_suffix) == 0) {

                printf("  Found component: %s\n", entry->d_name);
                fprintf(output, "#include \"%s\"\n", entry->d_name);
            }
        }
    }

    fprintf(output, "\n#endif\n");

    closedir(dir);
    fclose(output);

    printf("Successfully generated: %s\n", output_path);
    return 0;
}
