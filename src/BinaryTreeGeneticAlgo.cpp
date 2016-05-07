#include "../include/BinaryTreeGeneticAlgo.h"

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
	std::vector<BinaryTreeChromosome*>* newGeneration,
	std::vector<BinaryTreeChromosome*>* oldGeneration,
	unsigned size) {

	std::default_random_engine generator;
	std::uniform_int_distribution<unsigned> distribution(0, this->select);

	for (unsigned i = size - 1, y = 0; i >= size - this->select; i--, y++) {
		newGeneration->at(y)->setFitness(oldGeneration->at(i)->getFitness());
		oldGeneration->at(i)->copyTo(newGeneration->at(y));

	}

	for (unsigned i = this->select; i < size; i++) {
		unsigned index = distribution(generator);
		this->Mutate(newGeneration, index, newGeneration->at(i));
	}

	this->Crossover(newGeneration);
}

void BinaryTreeGeneticAlgo::Mutate(
	std::vector<BinaryTreeChromosome*>* generation,
	unsigned index,
	BinaryTreeChromosome * outputChromosome) {

	generation->at(index)->copyTo(outputChromosome);

	outputChromosome->Mutate(leafValueMutationProbability,
		leafValueSignMutationProbability,
		logicalNodeMutationProbability,
		crossoverProbability,
		leafIndicatorMutationProbability);
}

void BinaryTreeGeneticAlgo::Crossover(std::vector<BinaryTreeChromosome*>* generation) {
	for (unsigned long i = this->select + 1; i < generation->size(); i++) {
		double crossover = static_cast<double>(rand() % 100) / 100;

		if (crossover <= this->crossoverProbability) {
			unsigned long rnd = i;

			while (rnd == i || rnd <= this->select)
				rnd = rand() % generation->size();

			this->Crossover(generation->at(i), generation->at(rnd));
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
