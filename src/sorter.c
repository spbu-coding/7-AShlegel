#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_INIT_CAPACITY 16

#define MAX_STR_SIZE 1000
#define RETURN_ERROR(err_code, ...) {fprintf(stderr, __VA_ARGS__ "\n"); err_code; return 1;}

#define MAX_NUM_SRT_PRINT 100

static const char *WASTE_VAL = ",.;:!?\n";

int str_compare(const void *a, const void *b) { return strcmp(*(char *const *) b, *(char *const *) a); }

// ----------------------------------------------------------------------------------------------------------- Vector //

typedef struct vector {
    char **items;
    int capacity;
    int total;
} vector;

int vector_init(vector *v) {
    v->items = malloc(sizeof(char *) * VECTOR_INIT_CAPACITY);
    if (v->items == NULL) return 1;
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    return 0;
}

int vector_push(vector *v, char *item) {
    if (v->total == v->capacity) {
        v->capacity *= 2;
        char **tmp = realloc(v->items, sizeof(char *) * v->capacity);
        if (tmp == NULL) return 1;
        v->items = tmp;
    }
    v->items[v->total++] = item;
    return 0;
}

void vector_free(vector *v) {
    free(v->items);
    v->items = NULL;
}

#undef VECTOR_INIT_CAPACITY

// ------------------------------------------------------------------------------------------------------------- Read //

int is_not_waste(char c) {
    if (strchr(WASTE_VAL, c) == NULL) return 1;
    return 0;
}

int read_file(const char *filename, vector *v) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) RETURN_ERROR(, "File not found")
    if (vector_init(v)) RETURN_ERROR(, "Vector alloc error")


    char buf[MAX_STR_SIZE];
    while (fgets(buf, MAX_STR_SIZE, file)) {
        size_t buf_len = 0;
        size_t str_len = 0;
        for (buf_len = 0; buf[buf_len] != '\0'; buf_len++) {
            char c = buf[buf_len];
            if (is_not_waste(c)) {
                str_len++;
            }
        }
        char *str = malloc(str_len + 1);
        if (str == NULL) RETURN_ERROR(vector_free(v); fclose(file), "Memory alloc error")

        str[str_len] = '\0';

        for (int i = (int) buf_len - 1; i >= 0; i--){
            char c = buf[i];
            if (is_not_waste(c)) str[--str_len] = c;
        }
        if (vector_push(v, str)) RETURN_ERROR(vector_free(v); fclose(file), "Vector alloc error")
    }
    fclose(file);
    return 0;
}

#undef MAX_STR_SIZE
#undef RETURN_ERROR

// ------------------------------------------------------------------------------------------------------------ Print //

void print_first_str(vector str){
    size_t size = MAX_NUM_SRT_PRINT < str.total ? MAX_NUM_SRT_PRINT : str.total;
    for (size_t i = 0; i < size; i++){
        printf("%s\n", str.items[i]);
    }
}

#undef MAX_NUM_SRT_PRINT

// ------------------------------------------------------------------------------------------------------------- Main //

int main(int argc, char *argv[]) {
    if (argc == 2) {
        vector str_arr;
        if (!read_file(argv[1], &str_arr)) {
            qsort(str_arr.items, str_arr.total, sizeof(char*), str_compare);
            print_first_str(str_arr);
            vector_free(&str_arr);
            return 0;
        }
    } else fprintf(stderr, "Invalid args\n");
    return 1;
}
