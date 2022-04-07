#include "semver.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int compare_versions (SemanticVersion a, SemanticVersion b)
{
        if (a.major != b.major)
                return a.major < b.major ? -1 : 1;

        if (a.minor != b.minor)
                return a.minor < b.minor ? -1 : 1;

        if (a.patch != b.patch)
                return a.patch < b.patch ? -1 : 1;

        return 0;
}

char *parse_version_component (char *version, int *component_buf)
{
        char *dot = strchr (version, '.');

        if (!dot)
                return NULL;

        char *end;
        *dot = '\0';
        *component_buf = strtol (version, &end, 10);
        *dot = '.';

        if (end != dot)
                return NULL;

        return dot + 1;
}

bool parse_semantic_version (SemanticVersion *ver, char *version)
{
        if (!(version = parse_version_component (version, &ver->major)))
                return false;

        if (!(version = parse_version_component (version, &ver->minor)))
                return false;

        ver->patch = strtol (version, NULL, 10);

        return true;
}