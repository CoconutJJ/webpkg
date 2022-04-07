#ifndef registry_h
#define registry_h
#include "../semver/semver.h"
#include <stdlib.h>

/**
 * ISO Time
 */
typedef struct {
        int year;
        short month;
        short day;
        short hour;
        short minute;
        short second;
} ISOTime;

/**
 * Describes a package-version tuple
 */
typedef struct {
        char *name;
        SemanticVersion version;
} PackageLabel;

/**
 * Describes a person
 */
typedef struct {
        char *name;  /* Person's name */
        char *url;   /* Person's website */
        char *email; /* Person's email */
} Identity;

typedef struct {
        PackageLabel *deps;
        size_t capacity;
        size_t count;
} PackageDeps;

typedef struct {
        char *name;
        SemanticVersion version;
        char *tarball;
} PackageVersion;

typedef struct {
        char *name;

        PackageVersion *versions;
        size_t versions_capacity;
        size_t versions_count;

        ISOTime mtime;
        ISOTime ctime;

        Identity *maintainers;
        size_t maintainers_capacity;
        size_t maintainers_count;

        char *repository;
        char *url;
        char *description;

} Registry;

#endif