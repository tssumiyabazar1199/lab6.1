#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CUTOFF 15

// Файлаас оролт унших, гаралт бичихэд хурдтай байх үүднээс buffer ашиглах эсвэл стандарт I/O
static void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static void insertion_sort(int a[], int lo, int hi) {
    for (int i = lo + 1; i <= hi; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= lo && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

static void random_shuffle(int a[], int lo, int hi) {
    for (int i = lo; i <= hi; i++) {
        int r = i + rand() % (hi - i + 1);
        swap(&a[i], &a[r]);
    }
}

static void _single_pivot_qsort(int a[], int lo, int hi) {
    if (hi - lo <= CUTOFF) {
        insertion_sort(a, lo, hi);
        return;
    }

    int pivot = a[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (a[j] <= pivot) {
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[hi]);
    int pi = i + 1;

    _single_pivot_qsort(a, lo, pi - 1);
    _single_pivot_qsort(a, pi + 1, hi);
}

static void _dual_pivot_qsort(int a[], int lo, int hi) {
    if (hi - lo <= CUTOFF) {
        insertion_sort(a, lo, hi);
        return;
    }

    if (a[lo] > a[hi]) {
        swap(&a[lo], &a[hi]);
    }
    
    int p = a[lo], q = a[hi];
    int l = lo + 1, g = hi - 1, k = l;

    while (k <= g) {
        if (a[k] < p) {
            swap(&a[k], &a[l]);
            l++;
        } else if (a[k] >= q) {
            while (a[g] > q && k < g) {
                g--;
            }
            swap(&a[k], &a[g]);
            g--;
            if (a[k] < p) {
                swap(&a[k], &a[l]);
                l++;
            }
        }
        k++;
    }
    l--;
    g++;

    swap(&a[lo], &a[l]);
    swap(&a[hi], &a[g]);

    _dual_pivot_qsort(a, lo, l - 1);
    _dual_pivot_qsort(a, l + 1, g - 1);
    _dual_pivot_qsort(a, g + 1, hi);
}

// Гаднаас дуудагдах үндсэн функцүүд (static биш)
void single_pivot_qsort(int a[], int n) {
    srand(time(NULL));
    random_shuffle(a, 0, n - 1);
    _single_pivot_qsort(a, 0, n - 1);
}

void dual_pivot_qsort(int a[], int n) {
    srand(time(NULL));
    random_shuffle(a, 0, n - 1);
    _dual_pivot_qsort(a, 0, n - 1);
}

// 1,000,000 хүртэлх тоог файлтай маш хурдан харьцаж уншиж бичих main хэсэг
int main() {
    FILE *infile = fopen("input.txt", "r");
    if (!infile) {
        printf("input.txt файл олдсонгүй!\n");
        return 1;
    }

    int capacity = 1000000;
    int *a = (int *)malloc(capacity * sizeof(int));
    int n = 0;

    while (fscanf(infile, "%d", &a[n]) == 1) {
        n++;
        if (n >= capacity) {
            capacity *= 2;
            int *temp = (int *)realloc(a, capacity * sizeof(int));
            if (!temp) {
                free(a);
                fclose(infile);
                return 1;
            }
            a = temp;
        }
    }
    fclose(infile);

    // Энд аль аргыг ашиглахаа сонгож болно (Жишээ нь dual_pivot_qsort эсвэл single_pivot_qsort)
    dual_pivot_qsort(a, n);

    FILE *outfile = fopen("output.txt", "w");
    if (!outfile) {
        free(a);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fprintf(outfile, "%d\n", a[i]);
    }
    fclose(outfile);

    free(a);
    return 0;
}