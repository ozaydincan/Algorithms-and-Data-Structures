#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 21

typedef struct product {
  char *id;
  int price;
  char *model;
  struct product *next;
} product;

typedef struct brand {
  char *name;
  char *id;
  struct brand *next;
  struct product *products;
} brand;

brand *create_brand(char *name, char *id) {
  brand *new_brand = (brand *)malloc(sizeof(brand));
  if (new_brand == NULL) {
    fprintf(stderr, "Error creating brand node!\n");
    return NULL;
  }
  new_brand->name = strdup(name);
  new_brand->id = strdup(id);
  new_brand->products = NULL;
  new_brand->next = NULL;
  return new_brand;
}

product *create_product(char *id, char *model, int price) {
  product *new_product = (product *)malloc(sizeof(product));
  if (new_product == NULL) {
    fprintf(stderr, "Error creating product node!\n");
    return NULL;
  }
  new_product->id = strdup(id);
  new_product->model = strdup(model);
  new_product->price = price;
  new_product->next = NULL;
  return new_product;
}

int get_brands(const char *producer_file, brand **brands) {
  FILE *fp = fopen(producer_file, "r");
  if (fp == NULL) {
    perror("Producer file couldn't be opened!");
    return -1;
  }

  char name[MAX_LEN], id[MAX_LEN];
  brand *head = NULL, *current = NULL;

  while (fscanf(fp, "%s %s", name, id) == 2) {
    brand *new_brand = create_brand(name, id);
    if (new_brand == NULL) {
      fclose(fp);
      return -1;
    }
    if (head == NULL) {
      head = new_brand;
      current = head;
    } else {
      current->next = new_brand;
      current = new_brand;
    }
  }

  *brands = head;
  fclose(fp);
  return 0;
}

int add_product_to_brand(brand *brands, char *brand_id, char *model,
                         int price) {
  brand *current_brand = brands;

  while (current_brand != NULL) {
    if (strcmp(current_brand->id, brand_id) == 0) {
      product *new_product = create_product(brand_id, model, price);
      if (new_product == NULL) {
        return -1;
      }
      product *current_product = current_brand->products;

      if (current_product == NULL) {
        current_brand->products = new_product;
      } else {
        while (current_product->next != NULL) {
          current_product = current_product->next;
        }
        current_product->next = new_product;
      }
      return 0;
    }
    current_brand = current_brand->next;
  }
  return -1;
}

int get_products(const char *product_file, brand *brands) {
  FILE *fp = fopen(product_file, "r");
  if (fp == NULL) {
    perror("Product file couldn't be opened!");
    return -1;
  }

  char brand_id[MAX_LEN], model[MAX_LEN];
  int price;

  while (fscanf(fp, "%s %s %d", brand_id, model, &price) == 3) {
    if (add_product_to_brand(brands, brand_id, model, price) != 0) {
      fclose(fp);
      return -1;
    }
  }

  fclose(fp);
  return 0;
}

int display_products(brand *brands, char *brand_name) {
  brand *current = brands;

  while (current != NULL) {
    if (strcmp(current->name, brand_name) == 0) {
      printf("Products for %s:\n", current->name);
      product *current_product = current->products;
      while (current_product != NULL) {
        printf("Model: %s, Price: %d\n", current_product->model,
               current_product->price);
        current_product = current_product->next;
      }
      return 0;
    }
    current = current->next;
  }
  printf("Brand %s not found!\n", brand_name);
  return -1;
}

int free_brands(brand *head) {
  brand *current_brand = head;
  while (current_brand != NULL) {
    brand *temp_brand = current_brand;
    product *current_product = current_brand->products;

    while (current_product != NULL) {
      product *temp_product = current_product;
      free(current_product->id);
      free(current_product->model);
      current_product = current_product->next;
      free(temp_product);
    }

    free(current_brand->id);
    free(current_brand->name);
    current_brand = current_brand->next;
    free(temp_brand);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Insufficient CLA!\n");
    return EXIT_FAILURE;
  }

  brand *brands = NULL;

  if (get_brands(argv[1], &brands) != 0) {
    fprintf(stderr, "Error loading brands!\n");
    return EXIT_FAILURE;
  }

  if (get_products(argv[2], brands) != 0) {
    fprintf(stderr, "Error loading products!\n");
    free_brands(brands);
    return EXIT_FAILURE;
  }

  char input[MAX_LEN];
  while (1) {
    printf("Enter manufacturer name (or 'exit' to quit): ");
    scanf("%s", input);
    if (strcmp(input, "exit") == 0) {
      break;
    }
    if (display_products(brands, input) != 0) {
      fprintf(stderr, "Failed to display products for %s\n", input);
    }
  }

  free_brands(brands);
  return 0;
}
