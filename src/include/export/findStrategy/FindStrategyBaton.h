#ifndef _FIND_STRATEGY_BATON_H
#define _FIND_STRATEGY_BATON_H

#include <nan.h>
#include <node.h>

#include "../../BinaryTreeChromosome.h"

const int DEFAULT_POPULATION_COUNT = 100;
const int DEFAULT_GENERATION_COUNT = 100;
const int DEFAULT_SELECTION_AMOUNT = 10;

const double DEFAULT_LEAF_VALUE_MUTATION_PROBABILITY = 0.5;
const double DEFAULT_LEAF_SIGN_MUTATION_PROBABILITY = 0.3;
const double DEFAULT_LOGICAL_NODE_MUTATION_PROBABILITY = 0.3;
const double DEFAULT_LEAF_INDICATOR_MUTATION_PROBABILITY = 0.2;
const double DEFAULT_CROSSOVER_PROBABILITY = 0.03;
const double DEFAULT_SPREAD = 0.0;

class FindStrategyBaton
{
public:
	Nan::Callback* progress;
	BinaryTreeChromosome* chromosome;
	BinaryTreeChromosome* startingChromosome;
	std::vector<std::unordered_map<std::string, double>> input;
	std::vector<std::string> inputTypes;
	std::vector<double> closeValues;
	const char* errorMessage;
	unsigned populationCount;
	unsigned generationCount;
	unsigned selectionAmount;
	double leafValueMutationProbability;
	double leafSignMutationProbability;
	double logicalNodeMutationProbability;
	double leafIndicatorMutationProbability;
	double crossoverProbability;
	double spread;
	double pipInDecimals;

	FindStrategyBaton();
	void PopulateFrom(v8::Handle<v8::Object> configuration);
};

#endif
