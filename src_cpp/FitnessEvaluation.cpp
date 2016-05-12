#include "../include/TradingSystem.h"

inline double EvaluateFitness(FitnessFunctionArgs args)
{
	TradingSimulator simulator;
	std::vector<Trade>* trades = simulator.Simulate(args.chromosome, args.data);

	if (trades->size() == 0)
	{
		return 0;
	}

	double points = 0;

	for (unsigned long i = 0; i < trades->size(); i++)
	{
		int duration = trades->at(i).End->Time - trades->at(i).Start->Time;
		double spreadValue = args.pipInDecimals * args.spread;
		double revenue = trades->at(i).getRevenue() - spreadValue;

		if (revenue > 0)
		{
			points += 1.0 / duration;
		}
		else
		{
			points -= 3.5 / duration;
		}
	}

	double fitness = points;
	delete trades;

	return fitness;
}
