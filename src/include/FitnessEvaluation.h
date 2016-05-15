#ifndef _FITNESS_EVALUATION_H
#define _FITNESS_EVALUATION_H

/**
 * Fitness fucntion for chromosome quality evaluation
 *
 * @param  chromosome Pointer to evaluated chromosome
 * @param  data       input data containing generated indicator values
 * @return            fitness of the passed chromosome
 */
 inline double EvaluateFitness(FitnessFunctionArgs args)
 {
 	TradingSimulator simulator;
  
 	std::vector<Trade>* trades = simulator.Simulate(
    args.chromosome,
    args.closePrices,
    args.data);

 	if (trades->size() == 0)
 	{
 		return 0;
 	}

 	double points = 0;

 	for (unsigned long i = 0; i < trades->size(); i++)
 	{
 		int duration = trades->at(i).End - trades->at(i).Start;
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

#endif
