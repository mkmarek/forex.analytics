#ifndef _STRATEGY_UPDATE_BATON_H
#define _STRATEGY_UPDATE_BATON_H

struct StrategyUpdateBaton
{
	double fitness;
	int generation;
	BinaryTreeChromosome* chromosome;
};

#endif
