#include <stdio.h>
#include <stdlib.h>

#include "some_heap.h"

#define KEY_NOT_PRESENT -1

heap_t *heap_create(int capacity) {
    // allocate memory for the heap, initialize values
    heap_t *heap = malloc(sizeof(heap_t));
    heap->data = malloc(capacity * sizeof(heap_node_t));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heap_free(heap_t *heap) {
    // free the memory allocated for the heap
    free(heap->data);
    free(heap);
}

unsigned int heap_size(heap_t *heap) { return heap->size; }

unsigned int heap_parent(unsigned int index) { 
    return (index - 1) / 2; // because of how power of 2 works
 }

unsigned int heap_left_child(unsigned int index) { 
    return 2 * index + 1; // also due to power of 2, move to child row and inc 1
 }

unsigned int heap_right_child(unsigned int index) { 
    return 2 * index + 2; // as right nest to left child, add 1
 }

unsigned int heap_level(unsigned int index) {
    unsigned int level = 0;
    while (index > 0) {  // basically get parent until root, then check how many times we did it
        index = heap_parent(index);
        level++;
    }
    return level;
}

void heap_print(heap_t *heap) {
    for (int ix = 0; ix < heap_size(heap); ix++) {
        printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
               heap->data[ix].key);
    }
    printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2) {
    heap_node_t temp = heap->data[index1]; // store our data from 1
    heap->data[index1] = heap->data[index2]; // move 2 to 1 (real)
    heap->data[index2] = temp; // move 1 (temp) to 2
}

void heap_bubble_up(heap_t *heap, int index) { // deal with insertion
    // while the current node is smaller than its parent, swap them
    while (index > 0 && heap->data[index].key < heap->data[heap_parent(index)].key) {
        heap_swap(heap, index, heap_parent(index));
        index = heap_parent(index);
    }
}

void heap_bubble_down(heap_t *heap, int index) { // deal with removal
    // tracker for smallest
    int min_index = index;

    // find the smallest child of the current node
    if (heap_left_child(index) < heap_size(heap) &&
        heap->data[heap_left_child(index)].key < heap->data[min_index].key) {
        min_index = heap_left_child(index);
    }
    if (heap_right_child(index) < heap_size(heap) &&
        heap->data[heap_right_child(index)].key < heap->data[min_index].key) {
        min_index = heap_right_child(index);
    }

    // if the smallest child is smaller than the current node, swap them
    if (min_index != index) {
        heap_swap(heap, index, min_index);
        heap_bubble_down(heap, min_index);
    }
}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data) {
    if (heap_size(heap) == heap->capacity) {
        return;
    }

    heap->data[heap_size(heap)].key = key;
    heap->data[heap_size(heap)].value = data;
    heap->size++;

    heap_bubble_up(heap, heap_size(heap) - 1);
}


heap_value_t heap_remove_min(heap_t *heap) {
    if (heap_size(heap) == 0) {
        return NULL;
    }

    heap_value_t min = heap->data[0].value;

    heap->size--;

    // move last element to the root
    heap->data[0] = heap->data[heap_size(heap)];

    // then bubble it down to its correct position
    heap_bubble_down(heap, 0);

    return min;
}