#include "../../include/stdafx.h"
#include "../../include/utils/HeapSort.h"

HeapSort::HeapSort(){
}


HeapSort::~HeapSort(){
}


inline void shift_down(std::vector<BinaryTreeChromosome *>& heap, size_t i, size_t max){
	size_t i_big, c1, c2;

    while (i < max) {
        i_big = i;
        c1    = (2 * i) + 1;
        c2    = c1 + 1;
        if ((c1 < max) && (heap[c1]->getFitness() > heap[i_big]->getFitness())) {
            i_big = c1;
        }
        if ((c2 < max) && (heap[c2]->getFitness() > heap[i_big]->getFitness())) {
            i_big = c2;
        }
        if (i_big == i) {
            return;
        }
        std::swap(heap[i], heap[i_big]);
        i = i_big;
    }
}


inline void to_heap(std::vector<BinaryTreeChromosome *>& arr, size_t size){
	int i = static_cast<int>(size / 2) - 1;

    while (i >= 0) {
        shift_down(arr, i, size);
        --i;
    }
}


inline void heap_sort(std::vector<BinaryTreeChromosome *>& arr, size_t size){
    to_heap(arr, size);
	size_t end = size - 1;
    while (end > 0) {
        std::swap(arr[0], arr[end]);
        shift_down(arr, 0, end);
        --end;
    }
}

void HeapSort::Sort(std::vector<BinaryTreeChromosome *>& array, size_t size){
    to_heap(array, size);
	size_t end = size - 1;
    while (end > 0) {
        std::swap(array[0], array[end]);
        shift_down(array, 0, end);
        --end;
    }
}


void HeapSort::checkRootNode(std::vector<BinaryTreeChromosome *>& array, size_t root, size_t size){
    size_t left  = 2 * root;
    size_t right = 2 * root + 1;

    if ((left < size) && (array[root]->getFitness() < array[left]->getFitness())) {
        std::swap(array[root], array[left]);
        this->checkRootNode(array, left, size);
    }
    if ((right < size) && (array[root]->getFitness() < array[right]->getFitness())) {
        std::swap(array[root], array[right]);
        this->checkRootNode(array, right, size);
    }
}


void HeapSort::buildHeap(std::vector<BinaryTreeChromosome *>& array, size_t size){
    for (size_t i = size / 2; i > 0; --i) {
        this->checkRootNode(array, i, size);
    }
}


void HeapSort::heapSort(std::vector<BinaryTreeChromosome *>& array, size_t size){
    while (size > 0) {
        std::swap(array[1], array[size - 1]);
        this->checkRootNode(array, 1, --size);
    }
}
