#ifndef json_h
#define json_h
#include "parser.h"
JSON *json_parse (char *input);
void json_destroy (JSON *obj);
JSON_ENTRY *json_find_entry (JSON_OBJECT *obj, char *key);
char *json_find_entry_with_string_value (JSON_OBJECT *obj, char *key);
JSON_OBJECT *json_find_entry_with_obj_value (JSON_OBJECT *obj, char *key);
JSON_ENTRY *json_iterate_entries (JSON_OBJECT *obj, size_t *idx);
JSON *json_iterate_array (JSON_ARRAY *arr, size_t *idx);
#endif