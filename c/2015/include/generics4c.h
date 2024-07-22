#include <stdbool.h>
#include <stdlib.h>

typedef enum GenericValueType {
    TYPE_BOOL,
    TYPE_INT,
    TYPE_STRING,
    TYPE_OBJECT,
    TYPE_ARRAY
} GenericValueType;

typedef union {
    bool boolean_value;
    int int_value;
    char* string_value;
    struct GenericArray *array_value;
    struct GenericObject *object_value;
} GenericValueData;

typedef struct GenericValue {
    GenericValueData data;
    GenericValueType type;
    void (*destructor)(struct GenericValue*);
} GenericValue;

typedef struct GenericArray {
    GenericValue** elements;
    size_t size;
    size_t capacity;
} GenericArray;

typedef struct KeyValuePair {
    char *key;
    GenericValue* value;
} KeyValuePair;

typedef struct GenericObject {
    KeyValuePair** pairs;
    size_t size;
    size_t capacity;
} GenericObject;

GenericValue* generic_create_bool(bool value);

GenericValue* generic_create_int(int value);

GenericValue* generic_create_string(const char* value);

GenericObject* generic_create_object(size_t capacity);

void generic_add_to_object(GenericObject* generic_object, const char* key, GenericValue* generic_value);

KeyValuePair* generic_get_pair_from_object(GenericObject* generic_object, const char* key);

void generic_destroy_object(GenericObject* generic_object);

GenericArray* generic_create_array(size_t capacity);

void generic_add_to_array(GenericArray* generic_array, GenericValue* generic_value);

void generic_destroy_array(GenericArray* generic_array);

void generic_print_value(GenericValue* generic_value);

char* generic_value_to_string(GenericValue *gv);

bool generic_compare_value(GenericValue* gv1, GenericValue* gv2);

bool generic_compare_kvp(KeyValuePair* kvp1, KeyValuePair* kvp2);
