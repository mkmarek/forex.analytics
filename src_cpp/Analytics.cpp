#include <time.h>
#include <chrono>
#include <thread>

#include "../include/stdafx.h"
#include "../include/TradingSystem.h"
#include "../include/TradingSimulator.h"
#include "../include/utils/HeapSort.h"
#include "../include/fitnessEvaluation.h"


BinaryTreeChromosome* TradingSystem::PerformAnalysis(
	const std::vector<Candlestick>& candlesticks,
	const std::vector<BaseIndicator *>& indicators,
	unsigned populationCount,
	unsigned generationCount,
	unsigned selectionAmount,
	double leafValueMutationProbability,
	double leafSignMutationProbability,
	double logicalNodeMutationProbability,
	double leafIndicatorMutationProbability,
	double crossoverProbability,
	double pipInDecimals,
	double spread,
	BinaryTreeChromosome* chromosomeToStartWith,
	std::function<void(double fitness, BinaryTreeChromosome * chromosome, int generation)> update
)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	std::vector<IndicatorTuple> dataSet = EvaluateCandlesticks(candlesticks, indicators);

	// Initialization
	std::vector<BinaryTreeChromosome *> front_buffer = std::vector<BinaryTreeChromosome *>();
	std::vector<BinaryTreeChromosome *> back_buffer = std::vector<BinaryTreeChromosome *>();

	BinaryTreeFitness fitness(&(EvaluateFitness), &dataSet, pipInDecimals, spread);

	BinaryTreeGeneticAlgo selection = BinaryTreeGeneticAlgo(
		selectionAmount,
		leafValueMutationProbability,
		leafSignMutationProbability,
		logicalNodeMutationProbability,
		leafIndicatorMutationProbability,
		crossoverProbability
	);

	for (unsigned y = 0; y < populationCount; y++)
	{
		front_buffer.push_back(new BinaryTreeChromosome());
		back_buffer.push_back(new BinaryTreeChromosome());
	}


	for (unsigned i = 0; i < populationCount; i++)
	{
		front_buffer[i]->GenerateTree(3, indicators);
		back_buffer[i]->GenerateTree(3, indicators);

		front_buffer[i]->setFitness(0);
		back_buffer[i]->setFitness(0);
	}

 	if (chromosomeToStartWith != nullptr)
	{
		for (unsigned i = 0; i < populationCount; i++)
		{
			chromosomeToStartWith->copyTo(front_buffer[i]);
			chromosomeToStartWith->copyTo(back_buffer[i]);

			if (i != 0) {
				front_buffer[i]->Mutate(
					leafValueMutationProbability,
					leafSignMutationProbability,
					logicalNodeMutationProbability,
					crossoverProbability,
					leafIndicatorMutationProbability
				);

				back_buffer[i]->Mutate(
					leafValueMutationProbability,
					leafSignMutationProbability,
					logicalNodeMutationProbability,
					crossoverProbability,
					leafIndicatorMutationProbability
				);
			}
		}
	}


	HeapSort heapSort;

	std::vector<BinaryTreeChromosome*>* p_front_buffer = &front_buffer;
	std::vector<BinaryTreeChromosome*>* p_back_buffer = &back_buffer;
	std::vector<BinaryTreeChromosome*>* tmp2;

	for (unsigned y = 0; y < generationCount; y++)
	{
		fitness.CalculateFitness(p_front_buffer);

		tmp2 = p_front_buffer;
		p_front_buffer = p_back_buffer;
		p_back_buffer = tmp2;

		heapSort.Sort(p_back_buffer, populationCount);

		update(p_back_buffer->at(populationCount - 1)->getFitness(), p_back_buffer->at(populationCount - 1), y + 1 /* Start with 1 */);

		// Selection
		selection.Select(p_front_buffer, p_back_buffer, populationCount);
	}

	fitness.CalculateFitness(p_front_buffer);

	heapSort.Sort(p_front_buffer, populationCount);

	BinaryTreeChromosome* bestFit = new BinaryTreeChromosome(p_front_buffer->at(populationCount - 1));

	for (unsigned i = 0; i < populationCount; i++)
	{
		delete front_buffer[i];
		delete back_buffer[i];
	}

	return bestFit;
} // TradingSystem::PerformAnalysis
