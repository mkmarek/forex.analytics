#pragma once

#include "../stdafx.h"
#include "../models/Candlestick.h"

/**
 * A sign enum used for comparism of strategy required values
 * with real values
 */
enum Sign {
	Gt = 0,
	Lt = 1
};

/**
 * Structure containing data about genereated indicators
 * this is used mostly with the fitness function
 */
struct IndicatorData {

public:
	int offset;
	double data;
	Candlestick candlestick;
};

/**
 * The base indicator class desribing methods which all other classes used
 * to obtain data from indicators and evaluate them
 */
class BaseIndicator
{
public:
	static std::random_device r;

	std::mt19937 engine;
	std::uniform_real_distribution<double> uniform_dist;

	/**
	 * The indicator name
	 */
	std::string Name = "";

	/**
	 * Returns a vector of indicator values based on the input candlesticks
	 * @param  candlesticks The input candlesticks used for indicator calculation
	 * @param  startIndex   Defines the index from which the indicator starts
	 * @return              vector of douvles representing indicator values
	 */
	virtual std::vector<double> GetIndicatorData(const std::vector<Candlestick>* candlesticks, int * startIndex) = 0;

	/**
	 * Returns a random indicator value based on the upper and lower bonundaries
	 * set for this kind of indicator.
	 * @return Random possible indicator value
	 */
	virtual double getRandomValue() {
		return this->uniform_dist(engine);
	}

	BaseIndicator()
	{
		std::seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
		this->engine = std::mt19937(seed);
	}

protected:

	// fields and methods used when doing Ta-lib calls
	int startIdx;
	int endIdx;
	double * inOpen;
	double * inClose;
	double * inLow;
	double * inHigh;
	double * outReal;
	double * outReal2;
	double * outReal3;
	int * outInteger;

	/**
	 * Prepares the input parameters used by the ta-lib library based on the
	 * input candlesticks
	 * @param candlesticks Input candlesticks used for generating indicator data
	 */
	void PrepareParameters(const std::vector<Candlestick>* candlesticks) {
		this->startIdx = 0;
		this->endIdx = static_cast<int>(candlesticks->size()) - 1;

		size_t size = this->endIdx - this->startIdx + 1;

		this->inOpen = new double[size];
		this->inClose = new double[size];
		this->inLow = new double[size];
		this->inHigh = new double[size];

		this->outReal = new double[size];
		this->outReal2 = new double[size];
		this->outReal3 = new double[size];

		this->outInteger = new int[size];

		for (size_t i = 0; i < size; i++) {
			this->inOpen[i] = candlesticks->at(i).Open;
			this->inClose[i] = candlesticks->at(i).Close;
			this->inHigh[i] = candlesticks->at(i).High;
			this->inLow[i] = candlesticks->at(i).Low;
		}
	} // PrepareParameters

	/**
	 * Cleanup after indicator value generation
	 */
	void Clean() {
		delete this->inOpen;
		delete this->inClose;
		delete this->inLow;
		delete this->inHigh;

		delete this->outReal;
		delete this->outReal2;
		delete this->outReal3;
		delete this->outInteger;
	}
};
