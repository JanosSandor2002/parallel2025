#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

// ---------- Dinamikus tömb ----------
typedef struct {
    int *data;
    int size;
    int capacity;
    pthread_mutex_t lock;
} Array;

void array_init(Array *arr, int capacity) {
    arr->data = malloc(capacity * sizeof(int));
    arr->size = 0;
    arr->capacity = capacity;
    pthread_mutex_init(&arr->lock, NULL);
}

void array_push(Array *arr, int value) {
    pthread_mutex_lock(&arr->lock);
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(int));
    }
    arr->data[arr->size++] = value;
    pthread_mutex_unlock(&arr->lock);
}

void array_free(Array *arr) {
    free(arr->data);
    pthread_mutex_destroy(&arr->lock);
}

// ---------- Láncolt lista ----------
typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    pthread_mutex_t lock;
} List;

void list_init(List *list) {
    list->head = NULL;
    pthread_mutex_init(&list->lock, NULL);
}

void list_add(List *list, int value) {
    pthread_mutex_lock(&list->lock);
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->next = list->head;
    list->head = node;
    pthread_mutex_unlock(&list->lock);
}

int list_search(List *list, int value) {
    pthread_mutex_lock(&list->lock);
    Node *curr = list->head;
    while (curr) {
        if (curr->value == value) {
            pthread_mutex_unlock(&list->lock);
            return 1;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&list->lock);
    return 0;
}

void list_search_action(List *list, int value, void (*on_found)(int), void (*on_not_found)(int)) {
    pthread_mutex_lock(&list->lock);
    Node *curr = list->head;
    while (curr) {
        if (curr->value == value) {
            pthread_mutex_unlock(&list->lock);
            on_found(value);
            return;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&list->lock);
    on_not_found(value);
}

// CSV fájl írása
void list_to_csv(List *list, const char *filename) {
    pthread_mutex_lock(&list->lock);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Hiba a fájl megnyitásakor!\n");
        pthread_mutex_unlock(&list->lock);
        return;
    }
    
    Node *curr = list->head;
    while (curr) {
        fprintf(file, "%d\n", curr->value);
        curr = curr->next;
    }

    fclose(file);
    pthread_mutex_unlock(&list->lock);
}

void list_free(List *list) {
    pthread_mutex_lock(&list->lock);
    Node *curr = list->head;
    while (curr) {
        Node *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    pthread_mutex_unlock(&list->lock);
    pthread_mutex_destroy(&list->lock);
}

// ---------- Példák és mérések ----------
void* thread_add_to_list(void *arg) {
    List *list = (List *)arg;
    for (int i = 0; i < 1000; i++) {
        list_add(list, i);
    }
    return NULL;
}

void on_found(int val) {
    printf("Elem megtalálva: %d\n", val);
}

void on_not_found(int val) {
    printf("Elem NEM található: %d\n", val);
}

int main() {
    List list;
    list_init(&list);

    // Helytelen működés demonstráció (szálbiztosság nélkül)
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_add_to_list, &list);
    pthread_create(&t2, NULL, thread_add_to_list, &list);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Mérés
    clock_t start = clock();
    int found = list_search(&list, 500);
    clock_t end = clock();
    printf("Keresés eredménye: %d, idő: %.6f s\n", found, (double)(end - start) / CLOCKS_PER_SEC);

    // Callback-es keresés
    list_search_action(&list, 12345, on_found, on_not_found);

    // CSV fájl generálása
    list_to_csv(&list, "list_output.csv");

    list_free(&list);
    return 0;
}
