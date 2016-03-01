#pragma once
#include "stdafx.h"

#include "BinaryTreeFitness.h"
#include "models/Candlestick.h"
#include "BinaryTreeChromosome.h"

/**
 * Represents a model of one trade having a start and an end defined by
 * a specific candlestick.
 */
class Trade
{
public:

	/**
	 * Candlestic defining entry of this specific trade
	 */
	Candlestick Start;

	/**
	 * Candlestic defining exit of this specific trade
	 */
	Candlestick End;

	/**
	 * Is true when a trades was started with a buy action and false it with sell
	 */
	bool Buy;

	/**
	 * Defines the maximum loss during the trade
	 */
	double MaximumLoss;

	/**
	 * Defines the maximum proffit during the trade
	 */
	double MaximumProffit;

	/**
	 * Gets the total revenue of the performed trade
	 * @return double value in pips specifying the trade's revenue
	 */
	double getRevenue() const;

	/**
	 * Converts a vector of trades to a v8 array
	 * @param trades  The input vector if trades
	 * @param output  The output v8 array
	 * @param isolate The isolate object
	 */
	static void ToArray(
		const std::vector<Trade>& trades,
		v8::Local<v8::Array>& output,
		v8::Isolate* isolate);

	/**
	 * Converts an input trade to a v8 object
	 * @param trade   The input trade object
	 * @param output  The output v8 object
	 * @param isolate The isolate object
	 */
	static void ToObject(
		const Trade& trade,
		v8::Local<v8::Object>& output,
		v8::Isolate* isolate);
};


/**
 * Simulator simulates trading processes and generates trades based on
 * a defined strategy
 */
class TradingSimulator
{
public:

	/**
	 * Simulates a certain strategy on a set of test data and returns all performed \
	 * trades within the boundaries of the test data
	 * @param chromosome Chromosome containing trading strategy which is then
	 *                   applied on each indicator value in the input data
	 * @param data      Input data used for trade evaluation
	 * @return          Vector of trades made with passed strategy and test data
	 */
	std::vector<Trade> Simulate(
		const BinaryTreeChromosome* chromosome,
		const std::vector<IndicatorTuple>& data) const;
};
