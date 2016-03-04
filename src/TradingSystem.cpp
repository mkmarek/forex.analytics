#include "../include/stdafx.h"
#include "../include/TradingSystem.h"
#include "../include/TradingSimulator.h"

#include <time.h>

/**
 * Fitness fucntion for chromosome quality evaluation
 *
 * @param  chromosome Pointer to evaluated chromosome
 * @param  data       input data containing generated indicator values
 * @return            fitness of the passed chromosome
 */
double EvaluateFitness(FitnessFunctionArgs args)
{
	TradingSimulator simulator;
	std::vector<Trade> trades = simulator.Simulate(args.chromosome, args.data);

	double points = 0;

	for (unsigned long i = 0; i < trades.size(); i++)
	{
		double revenue = trades[i].getRevenue();
		int duration = trades[i].End.Time - trades[i].Start.Time;

		if (trades[i].MaximumLoss > 0.0010) {
			points -= 0.0010 / duration;
		}
		else if (revenue > 0.0005) //shorter trades than one day
		{ //at least 5 pips.
			//TODO: This is currently tailored for EUR/USD - will need to be changed
			points += revenue / duration;
		}
		else
		{
			points -= abs(revenue) / duration;
		}

	}

	return points;
}

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
	std::function<void(double fitness, BinaryTreeChromosome * chromosome, int generation)> update
)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	std::vector<IndicatorTuple> dataSet = EvaluateCandlesticks(candlesticks, indicators);

	// Initialization
	std::vector<BinaryTreeChromosome *> front_buffer = std::vector<BinaryTreeChromosome *>();
	std::vector<BinaryTreeChromosome *> back_buffer = std::vector<BinaryTreeChromosome *>();

	for (unsigned y = 0; y < populationCount; y++)
	{
		front_buffer.push_back(new BinaryTreeChromosome());
		back_buffer.push_back(new BinaryTreeChromosome());
	}

	BinaryTreeFitness fitness(&(EvaluateFitness), dataSet);

	BinaryTreeGeneticAlgo selection = BinaryTreeGeneticAlgo(
		selectionAmount,
		leafValueMutationProbability,
		leafSignMutationProbability,
		logicalNodeMutationProbability,
		leafIndicatorMutationProbability,
		crossoverProbability
	);

	for (unsigned i = 0; i < populationCount; i++)
	{
		front_buffer[i]->GenerateTree(4, indicators);
		back_buffer[i]->GenerateTree(4, indicators);
	}
	HeapSort heapSort;

	std::vector<BinaryTreeChromosome *>* p_front_buffer = &front_buffer;
	std::vector<BinaryTreeChromosome *>* p_back_buffer = &back_buffer;
	std::vector<BinaryTreeChromosome *>* tmp2;

	for (unsigned y = 0; y < generationCount; y++)
	{
		fitness.CalculateFitness(*p_front_buffer);

		tmp2 = p_front_buffer;
		p_front_buffer = p_back_buffer;
		p_back_buffer = tmp2;

		heapSort.Sort(*p_back_buffer, populationCount);

		update(p_back_buffer->at(populationCount - 1)->getFitness(), p_back_buffer->at(populationCount - 1), y + 1 /* Start with 1 */);

		// Selection
		selection.Select(*p_front_buffer, *p_back_buffer, populationCount);
	}

	fitness.CalculateFitness(*p_front_buffer);

	heapSort.Sort(*p_front_buffer, populationCount);

	BinaryTreeChromosome* bestFit = new BinaryTreeChromosome(p_front_buffer->at(populationCount - 1));

	for (unsigned i = 0; i < populationCount; i++)
	{
		delete front_buffer[i];
		delete back_buffer[i];
	}

	return bestFit;
} // TradingSystem::PerformAnalysis

std::vector<Candlestick> TradingSystem::ConvertOHLCToLargerTimeframe(
	const std::vector<Candlestick>& bars_to_convert,
	int seconds)
{
	std::vector<Candlestick> bars_converted;
	long current_tick_interval = -1;
	double current_bar_open = 0;
	double current_bar_high = 0;
	double current_bar_low = 0;
	double current_bar_close = 0;
	int boundary_adjusted_time = 0;

	if (bars_to_convert.size() == 0)
		return bars_converted;

	for (unsigned long i = 0; i < bars_to_convert.size(); i++)
	{
		Candlestick bar = bars_to_convert[i];

		int this_tick_interval = bar.Time / seconds;
		if (this_tick_interval != current_tick_interval)
		{
			if (current_tick_interval != -1)
			{
				Candlestick stick;

				stick.Time = boundary_adjusted_time;
				stick.Open = current_bar_open;
				stick.High = current_bar_high;
				stick.Low = current_bar_low;
				stick.Close = current_bar_close;


				bars_converted.push_back(stick);
			}

			current_tick_interval = this_tick_interval;
			boundary_adjusted_time = current_tick_interval * seconds;
			current_bar_open = bar.Open;
			current_bar_high = bar.High;
			current_bar_low = bar.Low;
			current_bar_close = bar.Close;
		}
		else
		{
			current_bar_high = bar.High > current_bar_high ? bar.High : current_bar_high;
			current_bar_low = bar.Low < current_bar_low ? bar.Low : current_bar_low;
			current_bar_close = bar.Close;
		}
	}
	// Add the final bar
	Candlestick stick;

	stick.Time = boundary_adjusted_time;
	stick.Open = current_bar_open;
	stick.High = current_bar_high;
	stick.Low = current_bar_low;
	stick.Close = current_bar_close;


	bars_converted.push_back(stick);

	return bars_converted;
} // TradingSystem::ConvertOHLCToLargerTimeframe

std::vector<IndicatorTuple> TradingSystem::EvaluateCandlesticks(
	const std::vector<Candlestick>& candlesticks,
	const std::vector<BaseIndicator *>& indicators)
{
	std::vector<IndicatorTuple> values;
	std::map<std::string, std::vector<IndicatorData>> indicatorValues;

	int start = 0;

	for (unsigned long i = 0; i < indicators.size(); i++)
	{
		BaseIndicator* indicator = indicators[i];

		int indicatorStart = 0;
		const std::vector<double>& indicatorValue = indicator->GetIndicatorData(candlesticks, &indicatorStart);

		std::vector<IndicatorData> data(indicatorValue.size());

		for (unsigned long y = 0; y < indicatorValue.size(); y++)
		{
			data[y].offset = indicatorStart;
			data[y].data = indicatorValue[y];
			data[y].candlestick = candlesticks[y + indicatorStart];
		}

		indicatorValues.insert(std::make_pair(indicator->Name, data));

		if (indicatorStart > start) start = indicatorStart;
	}

	for (unsigned long i = 0; i < indicators.size(); i++)
	{
		BaseIndicator* indicator = indicators[i];

		std::vector<IndicatorData>& data = indicatorValues[indicator->Name];

		// delete unnecessary values
		data.erase(data.begin(), data.begin() + (start - data.begin()->offset));
	}

	for (unsigned long y = 0; y < candlesticks.size() - start; y++)
	{
		std::map<std::string, IndicatorData> element;
		for (unsigned long i = 0; i < indicators.size(); i++)
		{
			element.insert(std::make_pair(indicators[i]->Name, (indicatorValues[indicators[i]->Name])[y]));
		}

		values.push_back(element);
	}

	return values;
} // TradingSystem::EvaluateCandlesticks
