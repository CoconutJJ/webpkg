#ifndef semver_h
#define semver_h
#include <stdbool.h>
typedef struct {
        int major;
        int minor;
        int patch;

} SemanticVersion;

int compare_versions (SemanticVersion a, SemanticVersion b);
char *parse_version_component (char *version, int *component_buf);
bool parse_semantic_version (SemanticVersion *ver, char *version);

#endif