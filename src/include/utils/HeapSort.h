#pragma once
#include <algorithm>
#include "../BinaryTreeChromosome.h"

class HeapSort
{
private:
void checkRootNode(std::vector<BinaryTreeChromosome *>* array, size_t root, size_t size);
void buildHeap(std::vector<BinaryTreeChromosome *>* array, size_t size);
void heapSort(std::vector<BinaryTreeChromosome *>* array, size_t size);

public:
HeapSort();
~HeapSort();
void Sort(std::vector<BinaryTreeChromosome *>* array, size_t size);
};
