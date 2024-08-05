#include <stdbool.h>
#include <stdlib.h>

typedef enum GenericValueType {
    GENERIC_TYPE_BOOL,
    GENERIC_TYPE_INT,
    GENERIC_TYPE_STRING,
    GENERIC_TYPE_OBJECT,
    GENERIC_TYPE_ARRAY
} GenericValueType;

typedef union {
    bool                    boolean_value;
    int                     int_value;
    char*                   string_value;
    struct GenericArray*    array_value;
    struct GenericObject*   object_value;
} GenericValueData;

typedef struct GenericArray GenericArray;
typedef struct GenericObject GenericObject;
typedef struct GenericValue GenericValue;
typedef struct KeyValuePair KeyValuePair;

// Constructors
GenericValue*   generic_create_bool(bool value);
GenericValue*   generic_create_int(int value);
GenericValue*   generic_create_string(const char* value);
GenericArray*   generic_create_array(size_t capacity);
GenericValue*   generic_create_array_value(GenericArray *value);
GenericObject*  generic_create_object(size_t capacity);
GenericValue*   generic_create_object_value(GenericObject *value);

// Destructors
void generic_destroy_array(GenericArray* generic_array);
void generic_destroy_object(GenericObject* generic_object);

// Getters
GenericArray*   generic_get_array(GenericValue *gv);
char*           generic_get_string(GenericValue *gv);
void*           generic_get_anything(GenericValue *gv);

// Arrays
size_t          generic_get_array_size(GenericArray *ga);
void            generic_add_to_array(GenericArray *ga, GenericValue *gv);
GenericValue*   generic_get_from_array(GenericArray *ga, size_t pos);

// Objects
void            generic_add_to_object(GenericObject* go, const char* key, GenericValue* gv);
GenericValue*   generic_get_from_object(GenericObject *go, const char *key);

// Values
GenericValueType    generic_get_type(GenericValue *gv);

// Deprecated
KeyValuePair*   generic_get_pair_from_object(GenericObject* go, const char* key);

// Convenience
void    generic_print_value(GenericValue *gv);
char*   generic_value_to_string(GenericValue *gv);
bool    generic_compare_value(GenericValue* gv1, GenericValue* gv2);
bool    generic_compare_kvp(KeyValuePair* kvp1, KeyValuePair* kvp2);
