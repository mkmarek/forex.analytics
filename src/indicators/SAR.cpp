#include "../../include/indicators/Indicator.h"
#include "../../include/indicators/factories/IndicatorFactory.h"

INDICATOR(SAR)(const std::vector<Candlestick>* candlesticks, int* startIndex)
{
  std::vector<double> indicatorData = std::vector<double>();

  //Initialize all required parameters
  this->PrepareParameters(candlesticks);

  int outNbElement;

  double optInMaximum = 0.2;
  double optInAcceleration = 0.02;

  unsigned int lookback = TA_SAR_Lookback(optInAcceleration, optInMaximum);

  if (candlesticks->size() < lookback) {
		return indicatorData;
	}

  TA_SAR(
    this->startIdx,
    this->endIdx,
    this->inHigh,
    this->inLow,
    optInAcceleration,
    optInMaximum,
    startIndex,
    &outNbElement,
    this->outReal);

  //Put the result into indicator data vector in RsiModel
  for (int i = 0; i < outNbElement; i++) {
    indicatorData.push_back(outReal[i]);
  }

  //Cleanup all required parameters
  this->Clean();

  return indicatorData;
}
