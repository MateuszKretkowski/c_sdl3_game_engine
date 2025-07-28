#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>

void set_cwd_to_project_root(void) {
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0) {
        printf("Failed to get executable path\n");
        return;
    }
    if (!PathRemoveFileSpecA(path)) return;
    if (!PathRemoveFileSpecA(path)) return;
    if (!SetCurrentDirectoryA(path)) {
        printf("Failed to set working directory\n");
        return;
    }
    printf("Working directory set to: %s\n", path);
}

#else

#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>

void set_cwd_to_project_root(void) {
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX - 1);
    if (count == -1) {
        perror("readlink");
        return;
    }
    path[count] = '\0';

    char *dir = dirname(path);

    char parent[PATH_MAX];
    strncpy(parent, dir, PATH_MAX);
    parent[PATH_MAX-1] = '\0';

    char *parent_dir = dirname(parent);

    if (chdir(parent_dir) != 0) {
        perror("chdir");
        return;
    }

    printf("Working directory set to: %s\n", parent_dir);
}
#endif
