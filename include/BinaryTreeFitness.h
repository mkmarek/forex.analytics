#pragma once
#include "stdafx.h"
#include "BinaryTreeChromosome.h"

/**
* Wrapping map with indicator ad a key name and IndicatorData representing value
*/
struct IndicatorTuple {
public:
	std::map<std::string, IndicatorData> Element;

	IndicatorTuple(
		std::map<std::string, IndicatorData>& element) {

		this->Element = element;
	}
};

/**
* Fitness function arguments
*/
struct FitnessFunctionArgs {
public:
	BinaryTreeChromosome* chromosome;
	const std::vector<IndicatorTuple>* data;

	FitnessFunctionArgs(
		BinaryTreeChromosome* chromosome,
		const std::vector<IndicatorTuple>* data) {

		this->chromosome = chromosome;
		this->data = data;
	}
};

/**
 * The fitness function signature.
 * This specifies how the function should look like
 */
typedef double(*FitnessFunction)(FitnessFunctionArgs args);

/**
 * Describes a fitness evaluator wrapping over a fitness function
 */
class BinaryTreeFitness
{
private:

	/**
	 * Pointer to a fitness function
	 */
	FitnessFunction eval;

	/**
	 * Dataset used for calculating fitness
	 */
	const std::vector<IndicatorTuple>* dataSet;

public:

	/**
	 * Creates a new instance of BinaryTreeFitness combining test values and
	 * fitness evaluation function
	 *
	 * @param  eval    Pointer to a fitness evaluation function
	 * @param  dataSet Vector of a pregenerated test data
	 */
	BinaryTreeFitness(
		FitnessFunction eval,
		const std::vector<IndicatorTuple>* dataSet
		);

	/**
	 * Calculates the fitnesss value for a specific set of chromosomes
	 * @param chromosomes Vector of potiners to a set of chromosomes
	 *                    forming a generation
	 */
	void CalculateFitness(
		const std::vector<BinaryTreeChromosome*>* chromosomes) const;
};
