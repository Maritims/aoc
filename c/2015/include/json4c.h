#include <stdbool.h>

#include "generics4c.h"

#define JSON_QUOTE '"'

typedef enum JsonTokenType {
    JSON_TOKEN_TYPE_COLON,
    JSON_TOKEN_TYPE_COMMA,
    JSON_TOKEN_TYPE_LEFT_BRACE,
    JSON_TOKEN_TYPE_LEFT_BRACKET,
    JSON_TOKEN_TYPE_NUMBER,
    JSON_TOKEN_TYPE_RIGHT_BRACE,
    JSON_TOKEN_TYPE_RIGHT_BRACKET,
    JSON_TOKEN_TYPE_STRING,
    JSON_TOKEN_TYPE_BOOL,
    JSON_TOKEN_TYPE_NULL
} JsonTokenType;

typedef struct {
    JsonTokenType type;
    GenericValue* value;
} JsonToken;

char* lex_string(char** str);

int* lex_number(char** str);

bool* lex_bool(char** str);

JsonToken* lex(char* str, size_t *out_length);

// Check first token. If it's a left brace we've got an object, if it's a left bracket we've got an array.
GenericValue* parse(JsonToken** tokens, bool is_root);

GenericArray* parse_array(JsonToken** tokens);

GenericObject* parse_object(JsonToken** tokens);

void json_parse_string(GenericValue **result, char* str);
