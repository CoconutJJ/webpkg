#include "json.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

JSON *json_parse (char *input)
{
        return parse_json (input);
}

void json_destroy (JSON *obj)
{
        destroy_json (obj);
}

JSON_ENTRY *json_find_entry (JSON_OBJECT *obj, char *key)
{
        for (int i = 0; i < obj->count; i++) {
                if (strcmp (obj->entries[i]->key, key)) {
                        return obj->entries[i];
                }
        }
        return NULL;
}

char *json_find_entry_with_string_value (JSON_OBJECT *obj, char *key)
{
        JSON_ENTRY *entry = json_find_entry (obj, key);

        if (!entry)
                return NULL;

        if (!JSONPTR_IS_ITEM (entry->value))
                return NULL;

        JSON_ITEM *item = AS_JSON_ITEM (entry->value);

        if (!JSONITEMPTR_IS_STRING (item))
                return NULL;

        return item->s;
}

JSON_OBJECT *json_find_entry_with_obj_value (JSON_OBJECT *obj, char *key)
{
        JSON_ENTRY *entry = json_find_entry (obj, key);

        if (!entry)
                return NULL;

        if (!JSONPTR_IS_OBJECT (entry->value))
                return NULL;

        JSON_OBJECT *target = AS_JSON_OBJECT (entry->value);

        return target;
}

JSON_ENTRY *json_iterate_entries (JSON_OBJECT *obj, size_t *idx)
{
        if (*idx >= obj->count)
                return NULL;

        JSON_ENTRY *entry = obj->entries[*idx];

        (*idx)++;

        return entry;
}

JSON *json_iterate_array (JSON_ARRAY *arr, size_t *idx)
{
        if (*idx >= arr->count)
                return NULL;

        JSON *item = arr->items[*idx];

        (*idx)++;

        return item;
}