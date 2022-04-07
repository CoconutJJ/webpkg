#include "registry.h"
#include "../arrays/array.h"
#include "../http/http.h"
#include "../json/json.h"
#include <stdlib.h>
#include <string.h>

static char *registry_endpoint = NULL;

void set_registry_endpoint (char *endpoint)
{
        registry_endpoint = endpoint;
}

char *construct_package_registry_url (char *package_name)
{
        int pkg_name_len = strlen (package_name);
        int registry_len = strlen (registry_endpoint);

        int url_buf_len = strlen (package_name) + strlen (registry_endpoint) + 2;

        char *pkg_url = malloc (url_buf_len);

        if (!pkg_url) {
                perror ("malloc");
                exit (EXIT_FAILURE);
        }

        pkg_url[0] = '\0';

        strcat (pkg_url, registry_endpoint);

        if (registry_endpoint[registry_len - 1] != '/')
                strcat (pkg_url, "/");

        strcat (pkg_url, package_name);

        return pkg_url;
}

void registry_add_version (Registry *registry, PackageVersion version)
{
        ARRAY_UPDATE_SIZE (registry->versions, PackageVersion, registry->versions_count, &registry->versions_capacity);

        registry->versions[registry->versions_count++] = version;
}

void parse_registry_version_json (PackageVersion *buffer, JSON_ENTRY *version)
{
        
}

bool fetch_registry_package (Registry *registry, char *package_name)
{
        HTTPRequest request;

        char *url = construct_package_registry_url (package_name);

        init_HTTPRequest (&request, url);

        http_get (&request);

        JSON *registry_info = json_parse (request.response_data.data);

        if (!registry_info)
                return false;

        char *pkg_name = json_find_entry_with_string_value (registry_info, "name");

        if (!pkg_name)
                return false;

        registry->name = strdup (pkg_name);

        char *description = json_find_entry_with_string_value (registry_info, "description");

        if (!description)
                return false;

        registry->description = strdup (description);

        JSON_OBJECT *versions = json_find_entry_with_obj_value (registry_info, "versions");

        if (!versions)
                return false;

        JSON_ENTRY *curr_version;
        size_t idx;
        while ((curr_version = json_iterate_entries (versions, &idx))) {
                PackageVersion version;
        }
}
