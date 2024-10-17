#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 21

char **merge(char **arr1, size_t len1, char **arr2, size_t len2);
void free_matrix(char ***matrix, size_t R, size_t C);
int write_output(const char *filename, size_t len, char **merged);
char ***readCities(const char *filename, size_t *count, size_t *R, size_t *C);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char ***matrix;
    size_t count, R, C;

    matrix = readCities(argv[1], &count, &R, &C);

    char **merged = matrix[0];
    size_t merged_size = C;

    for (size_t i = 1; i < R; i++) {
        char **new_merged = merge(merged, merged_size, matrix[i], C);
        if (i > 1) {
            free(merged);
        }
        merged = new_merged;
        merged_size += C;
    }

    if (write_output(argv[2], count, merged) != EXIT_SUCCESS) {
        perror("Failure to write output!");
        return EXIT_FAILURE;
    }

    free_matrix(matrix, R, C);
    free(merged);

    return EXIT_SUCCESS;
}

char ***readCities(const char *filename, size_t *count, size_t *R, size_t *C) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%zu %zu", R, C);
    *count = (*R) * (*C);

    char ***matrix = (char ***)malloc((*R) * sizeof(char **));
    if (!matrix) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_STR_LEN];

    for (size_t i = 0; i < *R; i++) {
        matrix[i] = (char **)malloc((*C) * sizeof(char *));
        if (!matrix[i]) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        for (size_t j = 0; j < *C; j++) {
            fscanf(fp, "%s", buffer);
            matrix[i][j] = strdup(buffer);
            if (!matrix[i][j]) {
                perror("Memory allocation failed for string");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(fp);
    return matrix;
}

char **merge(char **arr1, size_t len1, char **arr2, size_t len2) {
    char **result = (char **)malloc((len1 + len2) * sizeof(char *));
    if (!result) {
        perror("Memory allocation failed for merge");
        exit(EXIT_FAILURE);
    }

    size_t i = 0, j = 0, k = 0;

    while (i < len1 && j < len2) {
        if (strcmp(arr1[i], arr2[j]) < 0) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }

    while (i < len1) {
        result[k++] = arr1[i++];
    }

    while (j < len2) {
        result[k++] = arr2[j++];
    }

    return result;
}

int write_output(const char *filename, size_t len, char **merged) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error opening output file");
        return EXIT_FAILURE;
    }

    fprintf(fp, "%zu\n", len);
    for (size_t i = 0; i < len; i++) {
        fprintf(fp, "%s\n", merged[i]);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

void free_matrix(char ***matrix, size_t R, size_t C) {
    for (size_t i = 0; i < R; i++) {
        for (size_t j = 0; j < C; j++) {
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
}
