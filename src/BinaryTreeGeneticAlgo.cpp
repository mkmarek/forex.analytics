#include "../include/BinaryTreeGeneticAlgo.h"
#include "../include/nodes/OperatorTreeNode.h"
#include "../include/nodes/IndicatorTreeNode.h"

BinaryTreeGeneticAlgo::BinaryTreeGeneticAlgo(
	int selectNumber,
	double leafValueMutation,
	double leafValueSignMutation,
	double logicalNodeMutation,
	double leafValueIndicatorMutation,
	double crossover) {
	this->select = selectNumber;
	this->leafValueMutationProbability = leafValueMutation;
	this->leafValueSignMutationProbability = leafValueSignMutation;
	this->logicalNodeMutationProbability = logicalNodeMutation;
	this->crossoverProbability = crossover;
	this->leafIndicatorMutationProbability = leafValueIndicatorMutation;
}

BinaryTreeGeneticAlgo::~BinaryTreeGeneticAlgo() {
}

void BinaryTreeGeneticAlgo::Select(
	std::vector<BinaryTreeChromosome *>& newGeneration,
	std::vector<BinaryTreeChromosome *>& oldGeneration,
	HeapSort& heapSort,
	unsigned size) {
	heapSort.Sort(oldGeneration, size);

	for (unsigned i = size - 1, y = 0; i >= size - this->select; i--, y++) {
		newGeneration[y]->setFitness(oldGeneration[i]->getFitness());

		oldGeneration[i]->buy->Copy(newGeneration[y]->buy);
		oldGeneration[i]->sell->Copy(newGeneration[y]->sell);
	}

	for (unsigned i = this->select; i < size; i++) {
		this->Mutate(newGeneration, this->select, newGeneration[i]);
	}

	this->Crossover(newGeneration);
}

unsigned factorial(unsigned n) {
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

void BinaryTreeGeneticAlgo::Mutate(
	std::vector<BinaryTreeChromosome *>& generation,
	unsigned size,
	BinaryTreeChromosome * outputChromosome) {
	unsigned maxRand = factorial(size);
	unsigned rnd = rand() % maxRand;
	unsigned index = 0;

	for (; index < size && factorial(index) < rnd; index++) {
	}

	generation[index]->buy->Copy(outputChromosome->buy);
	generation[index]->sell->Copy(outputChromosome->sell);

	this->Mutate(outputChromosome->buy);
	this->Mutate(outputChromosome->sell);
}

void BinaryTreeGeneticAlgo::Mutate(TreeNode * node) {
	if (node->left != nullptr && node->right != nullptr) {
		OperatorTreeNode * opNode = static_cast<OperatorTreeNode *>(node);

		double logicalNodeMutation = static_cast<double>(rand() % 100) / 100;

		if (logicalNodeMutation <= logicalNodeMutationProbability) {
			opNode->GenerateRandomValue();
		}

		this->Mutate(opNode->left);
		this->Mutate(opNode->right);
	}
	else {
		IndicatorTreeNode * inNode = static_cast<IndicatorTreeNode *>(node);

		double leafValueMutation = static_cast<double>(rand() % 100) / 100;
		double leafValueSignMutation = static_cast<double>(rand() % 100) / 100;
		double leafValueIndicatorMutation = static_cast<double>(rand() % 100) / 100;

		if (leafValueMutation <= this->leafValueMutationProbability) {
			inNode->GenerateRandomValue();
		}

		if (leafValueIndicatorMutation <= this->leafIndicatorMutationProbability) {
			inNode->GenerateRandomIndicator();
		}

		if (leafValueSignMutation <= this->leafValueSignMutationProbability) {
			inNode->GenerateRandomSign();
		}

	}
} // BinaryTreeGeneticAlgo::Mutate

void BinaryTreeGeneticAlgo::Crossover(std::vector<BinaryTreeChromosome *>& generation) {
	for (unsigned long i = this->select + 1; i < generation.size(); i++) {
		double crossover = static_cast<double>(rand() % 100) / 100;

		if (crossover <= this->crossoverProbability) {
			unsigned long rnd = i;

			while (rnd == i || rnd <= this->select)
				rnd = rand() % generation.size();

			this->Crossover(generation[i], generation[rnd]);
		}
	}
}


void BinaryTreeGeneticAlgo::Crossover(
	BinaryTreeChromosome * left,
	BinaryTreeChromosome * right) {
	TreeNode * leftBuy = left->buy;

	left->buy = right->buy;
	right->buy = leftBuy;
}
