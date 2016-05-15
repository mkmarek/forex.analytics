#include "../../include/utils/HeapSort.h"

HeapSort::HeapSort(){
}


HeapSort::~HeapSort(){
}


inline void shift_down(std::vector<BinaryTreeChromosome *>* heap, size_t i, size_t max){
	size_t i_big, c1, c2;

    while (i < max) {
        i_big = i;
        c1    = (2 * i) + 1;
        c2    = c1 + 1;
        if ((c1 < max) && (heap->at(c1)->getFitness() > heap->at(i_big)->getFitness())) {
            i_big = c1;
        }
        if ((c2 < max) && (heap->at(c2)->getFitness() > heap->at(i_big)->getFitness())) {
            i_big = c2;
        }
        if (i_big == i) {
            return;
        }
        std::swap(heap->at(i), heap->at(i_big));
        i = i_big;
    }
}


inline void to_heap(std::vector<BinaryTreeChromosome *>* arr, size_t size){
	int i = static_cast<int>(size / 2) - 1;

    while (i >= 0) {
        shift_down(arr, i, size);
        --i;
    }
}

void HeapSort::Sort(std::vector<BinaryTreeChromosome *>* array, size_t size){
    to_heap(array, size);
	size_t end = size - 1;
    while (end > 0) {
        std::swap(array->at(0), array->at(end));
        shift_down(array, 0, end);
        --end;
    }
}


void HeapSort::checkRootNode(std::vector<BinaryTreeChromosome *>* array, size_t root, size_t size){
    size_t left  = 2 * root;
    size_t right = 2 * root + 1;

    if ((left < size) && (array->at(root)->getFitness() < array->at(root)->getFitness())) {
        std::swap(array->at(root), array->at(left));
        this->checkRootNode(array, left, size);
    }
    if ((right < size) && (array->at(root)->getFitness() < array->at(right)->getFitness())) {
        std::swap(array->at(root), array->at(right));
        this->checkRootNode(array, right, size);
    }
}


void HeapSort::buildHeap(std::vector<BinaryTreeChromosome *>* array, size_t size){
    for (size_t i = size / 2; i > 0; --i) {
        this->checkRootNode(array, i, size);
    }
}


void HeapSort::heapSort(std::vector<BinaryTreeChromosome *>* array, size_t size){
    while (size > 0) {
        std::swap(array->at(1), array->at(size - 1));
        this->checkRootNode(array, 1, --size);
    }
}
