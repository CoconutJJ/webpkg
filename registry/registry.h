#ifndef registry_h
#define registry_h
#include "../semver/semver.h"
#include <stdlib.h>
typedef struct {
        char *name;
        SemanticVersion version;
} PackageLabel;

typedef struct {
        char *name;
        char *url;

} PackageAuthor;

typedef struct {
        PackageLabel *deps;
        size_t capacity;
        size_t count;
} PackageDeps;

typedef struct {
        SemanticVersion version;
        char *name;
        char *description;
        char *homepage;
        PackageAuthor author;
        char *license;
} PackageVersion;

typedef struct {
        char *name;
        char *description;

        PackageVersion *versions;
        size_t versions_capacity;
        size_t versions_count;
        

} Registry;

#endif