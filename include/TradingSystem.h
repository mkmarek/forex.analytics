#pragma once
#include "stdafx.h"

#include "models/Candlestick.h"
#include "BinaryTreeChromosome.h"
#include "indicators/Indicator.h"
#include "BinaryTreeFitness.h"
#include "BinaryTreeGeneticAlgo.h"

/**
 * This is the entry point to the whole calculation systems
 * TradingSystem class groups all the more specific classes to one simple
 * interface
 */
class TradingSystem
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
 * @param  outFitness                       The maximum fitness value achieved

 * @return                                  The best fit chromosome found
 */
BinaryTreeChromosome* PerformAnalysis(
        const std::vector<Candlestick>& candlesticks,
        const std::vector<BaseIndicator*>& indicators,
        unsigned populationCount,
        unsigned generationCount,
        unsigned selectionAmount,
        double leafValueMutationProbability,
        double leafSignMutationProbability,
        double logicalNodeMutationProbability,
        double leafIndicatorMutationProbability,
        double crossoverProbability,
        double* outFitness);

/**
 * Converts candlestick values to giher timeframe specified by second interval
 *
 * @param  bars_to_convert             The input vector of candlestick that shall
 *                                     be converted
 *
 * @param  seconds                     The timeframe specified in seconds to which
 *                                     the ohlc vector shall be converted to
 *
 * @return                             New vector of candlesticks as a result
 *                                     of defined conversion
 */
std::vector<Candlestick> ConvertOHLCToLargerTimeframe(
        const std::vector<Candlestick>& bars_to_convert,
        int seconds);

/**
 * Evaluates the given candlesticks and indicatros and returns data for all
 * specified indicators
 *
 * @param  candlesticks             Input vector of candlesticks
 *
 * @param  indicators               Defined indicators
 * 
 * @return                          Vector of data assigned to a different
 *                                  indicators in a map
 */
std::vector<IndicatorTuple> EvaluateCandlesticks(
        const std::vector<Candlestick>& candlesticks,
        const std::vector<BaseIndicator*> &indicators);
};
