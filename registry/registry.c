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
        // calculate the total length of the url with space for an extra `/`
        // if needed
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

bool parse_registry_version_json (PackageVersion *buffer, JSON_ENTRY *version)
{
        // get semantic version
        if (!parse_semantic_version (&buffer->version, version->key))
                return false;

        // ensure that the entry is a object
        if (!JSONPTR_IS_OBJECT (version->value))
                return false;

        JSON_OBJECT *obj = AS_JSON_OBJECT (version->value);

        // copy name
        char *name = json_find_entry_with_string_value (obj, "name");

        if (!name)
                return false;

        buffer->name = strdup (name);

        // copy tarball url
        JSON_OBJECT *dist = json_find_entry_with_obj_value (obj, "dist");

        if (!dist)
                return false;

        char *tarball = json_find_entry_with_string_value (dist, "tarball");

        if (!tarball)
                return false;

        buffer->tarball = strdup (tarball);
        return true;
}

/**
 * Parses the redirect JSON object returned by a registry GET request.
 * 
 * Returns a shallow copy to the `location` JSON property url.  
 * @param payload JSON payload
 * @return location URL
 */
const char *parse_redirect_payload (char *payload)
{
        JSON *obj = parse_json (payload);

        if (!obj)
                return NULL;

        JSON_ENTRY *location = json_find_entry_with_string_value (obj, "location");

        if (!location)
                return NULL;

        if (!JSONPTR_IS_ITEM (location->value))
                return NULL;

        JSON_ITEM *item = location->value;

        if (!JSONITEMPTR_IS_STRING (item))
                return NULL;

        return item->s;
}

/**
 * Makes a GET request to the registry for the specified `package_name`.
 * 
 * Returns the request handler.
 * 
 * request.url field will always be set to null for handlers returned from this
 * function.  
 * 
 * @param package_name name of the package 
 * @return HTTPRequest request handler
 */
HTTPRequest package_http_request (char *package_name)
{
        HTTPRequest request;
        char *url = construct_package_registry_url (package_name);

        init_HTTPRequest (&request, url);
        http_get (&request);

        free (url);
        request.url = NULL;

        return request;
}

char *fetch_package_registry_data (char *package_name)
{
        HTTPRequest request = package_http_request (package_name);
        while (1) {
                if (request.http_response_code == 302) {
                        char *url = parse_redirect_payload (request.response_data.data);
                        destroy_HTTPRequest (&request);
                        init_HTTPRequest (&request, url);
                        http_get (&request);
                } else {
                        char *data = malloc (request.response_data.count + 1);
                        if (!data) {
                                perror ("malloc");
                                exit (EXIT_FAILURE);
                        }

                        memcpy (data, request.response_data.data, request.response_data.count);
                        destroy_HTTPRequest (&request);

                        return data;
                }
        }
}

bool fetch_registry_package (Registry *registry, char *package_name)
{
        char *registry_data = fetch_package_registry_data (package_name);

        JSON *registry_info = json_parse (registry_data);

        if (!registry_info)
                return false;

        char *pkg_name = json_find_entry_with_string_value (registry_info, "name");

        if (!pkg_name)
                return false;

        registry->name = strdup (pkg_name);

        JSON_OBJECT *versions = json_find_entry_with_obj_value (registry_info, "versions");

        if (!versions)
                return false;

        JSON_ENTRY *curr_version;
        size_t idx = 0;

        while ((curr_version = json_iterate_entries (versions, &idx))) {
                PackageVersion version;

                if (parse_registry_version_json (&version, curr_version))
                        registry_add_version (registry, version);
        }

        destroy_json (registry_info);
}
