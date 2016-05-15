#ifndef _ANALYTICS_H
#define _ANALYTICS_H

#include <functional>
#include <unordered_map>

#include "BinaryTreeChromosome.h"
#include "BinaryTreeFitness.h"
#include "BinaryTreeGeneticAlgo.h"

/**
 * This is the entry point to the whole calculation systems
 * TradingSystem class groups all the more specific classes to one simple
 * interface
 */
class Analytics
{
public:

/**
 * Performs an analysis over specified candlestick set using specified indicators
 * and options and returns the best fit strategy embedded in BinaryTreeChromosome
 * class
 *
 * @param  candlesticks                     An input vector of candlesticks used
 *                                          for whole analytis. The more the better
 *                                          but also slower
 *
 * @param  indicators                       An input vector of configured
 *                                          indicators used for creating
 *                                          strategies
 *
 * @param  populationCount                  The total population count within
 *                                          one generation
 *
 * @param  generationCount                  The total generation count
 *
 * @param  selectionAmount                  How many chromosomes shall be
 *                                          selected from the old generation
 *                                          when constructing a new one
 *
 * @param  leafValueMutationProbability     probability for changing the leaf
 *                                          value (0.0 - 1.0)
 *
 * @param  leafSignMutationProbability      probabolity for changing the leaf
 *                                          sign (0.0 - 1.0)
 *
 * @param  logicalNodeMutationProbability   probabolity for changing the logica
 *                                          node (0.0 - 1.0)
 *
 * @param  leafIndicatorMutationProbability probability for changing the leaf
 *                                          indicator  (0.0 - 1.0)
 *
 * @param  crossoverProbability             Probability for performing crossover
 *                                           (0.0 - 1.0)
 *
 * @param  spread             Spread that should be taken into account when
 *                            performing trade estimations. Specify it in pips
 *
 * @param  pipInDecimals      Pip represented in decimals. For EUR/USD it
 *                            would be for example: 0.0001
 *
 * @param  update							Lambda expression called during progress update

 * @return                                  The best fit chromosome found
 */
BinaryTreeChromosome* PerformAnalysis(
        const std::vector<double>& closeValues,
        const std::vector<std::string>& indicators,
        const std::vector<std::unordered_map<std::string, double>> indicatorData,
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
        std::function<void(
          double fitness,
          BinaryTreeChromosome * chromosome,
          int generation)> update);
};

#endif
