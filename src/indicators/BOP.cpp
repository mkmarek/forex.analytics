#include "../../include/indicators/Indicator.h"
#include "../../include/indicators/factories/IndicatorFactory.h"

INDICATOR(BOP)(const std::vector<Candlestick>* candlesticks, int* startIndex)
{
	std::vector<double> indicatorData = std::vector<double>();

	int outBeginIdx;
	int outNbElement;

	//Initialize all required parameters
	this->PrepareParameters(candlesticks);

	unsigned int lookback = TA_BOP_Lookback();

  if (candlesticks->size() <= lookback) {
		return indicatorData;
	}

	//Perform the CCI calculation
	TA_BOP(
		this->startIdx,
		this->endIdx,
		this->inOpen,
		this->inHigh,
		this->inLow,
		this->inClose,
		&outBeginIdx,
		&outNbElement,
		this->outReal);

	//Put the result into indicator data vector in RsiModel
	for (int i = 0; i < outNbElement; i++) {

		indicatorData.push_back(outReal[i]);
	}

	*startIndex = outBeginIdx;

	//Cleanup all required parameters

	this->Clean();

	return indicatorData;
}
