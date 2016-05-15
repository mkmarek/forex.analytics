#pragma once

#include <unordered_map>

#include "nodes/TreeNode.h"


/**
 * Class that specifies exactly one strategy which can be evaluated.
 * This chromosome class is a base building block for the genetic algorithm
 */
class BinaryTreeChromosome
{

private:

	/**
	 * A vector of pointers to indicators used for further data evaluation
	 */
	std::vector<std::string> indicators;

	/**
	 * Specifies the quality of a specific buy and sell combination and is
	 * used for comparism which chromosome is better suited for survival
	 * at each generation
	 */
	double fitness;

	/**
	 * Generates a single layer of tree nodes
	 * @param  index  Index of the layer
	 * @param  height The total height of the tree
	 * @return        Pointer to a new instance of tree node
	 */
	TreeNode * GenerateTree(int index, int height) const;

public:

	/**
	* Entry point to a tree structure defining strategy for selling
	*/
	TreeNode* buy;

	/**
	* Entry point to a tree structure defining strategy for buying
	*/
	TreeNode* sell;

	/**
	 * Creates a new instance of BinaryTreeChromosome by copying data from the
	 * the BinaryTreeChromosome instance passed as a parameter
	 */
	BinaryTreeChromosome(const BinaryTreeChromosome * chromosome);

	BinaryTreeChromosome();

	~BinaryTreeChromosome();


	void copyTo(BinaryTreeChromosome * chromosome) const;

	/**
	 * Generates the buy and sell binary trees based on the specified
	 * height and indicators defined
	 *
	 * @param height     Specifies how deep the generated tree shall be
	 *
	 * @param indicators Vector of indicators used in the final indicator
	 *                   tree nodes
	 */
	void GenerateTree(int height, const std::vector<std::string>& indicators);

	void Mutate(
		double leafValueMutationProbability,
		double leafValueSignMutationProbability,
		double logicalNodeMutationProbability,
		double crossoverProbability,
		double leafIndicatorMutationProbability,
		TreeNode* node);

	void Mutate(
		double leafValueMutationProbability,
		double leafValueSignMutationProbability,
		double logicalNodeMutationProbability,
		double crossoverProbability,
		double leafIndicatorMutationProbability
		);

	bool shouldBuy(const std::unordered_map<std::string, double>& data) const;

	bool shouldSell(const std::unordered_map<std::string, double>& data) const;

	/**
	 * Gets the fitness if this chromosome
	 * @return The fitness value of an instance of this chromosome
	 */
	double getFitness() const;

	/**
	 * Sets the fitness value of this chromosome
	 * @param value A value that shall be set as a fitness for this chromosome
	 */
	void setFitness(double value);

	/**
	 * Returns a reference to the indicator vector
	 */
	const std::vector<std::string>& getIndicators() const;

	/**
	 * Explicit operator for comparing fitness values for two chromosomes
	 */
	bool operator <(const BinaryTreeChromosome * chromosome) const;

	/**
	 * Explicit operator for comparing fitness values for two chromosomes
	 */
	bool operator >(const BinaryTreeChromosome * chromosome) const;

	/**
	 * Converts a v8 object to BinaryTreeChromosome instance
	 *
	 * @param  indicators vector of indicators used for this chromosome
	 * @param  input      The input v8 object
	 * @return            Pointer to an instance of a BinaryTreeChromosome
	 *                    This is created dynamically. So  it has to be deleted as
	 *                    well
	 */
	static BinaryTreeChromosome * FromJs(
		const std::vector<std::string>& indicators,
		const v8::Local<v8::Object>& input);


	void ToJs(v8::Handle<v8::Object>& input) const;
};
