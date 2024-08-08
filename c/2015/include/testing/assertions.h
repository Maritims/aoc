#ifndef TESTING_ASSERTIONS_H
#define TESTING_ASSERTIONS_H

#define print_error_and_exit(fmt, ...) \
    printf("%s failed: " fmt, __func__, __VA_ARGS__); \
    exit(EXIT_FAILURE);

#define assert_not_null(t1, fmt, ...) \
    if(t1 == NULL) { \
        print_error_and_exit(fmt, __VA_ARGS__); \
    }

#define assert_primitive_equality(t1, t2, fmt, ...) \
    if(t1 != t2) { \
        print_error_and_exit(fmt, __VA_ARGS__); \
    }

#define assert_string_equality(s1, s2, fmt, ...) \
    if(strcmp(s1, s2) != 0) { \
        print_error_and_exit(fmt, __VA_ARGS__); \
    }

#define assert_true(t1, fmt, ...) \
    if(!t1) { \
        print_error_and_exit(fmt, __VA_ARGS__); \
    }

#endif
