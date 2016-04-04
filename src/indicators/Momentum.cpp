#include "../../include/indicators/Indicator.h"
#include "../../include/indicators/factories/IndicatorFactory.h"

INDICATOR(Momentum)(const std::vector<Candlestick>* candlesticks, int* startIndex)
{
  std::vector<double> indicatorData = std::vector<double>();

  int outBeginIdx;
  int outNbElement;
  int optInTimePeriod = 14;

  if (candlesticks->size() < optInTimePeriod) {
    return indicatorData;
  }

  //Initialize all required parameters
  this->PrepareParameters(candlesticks);

  //Perform the Momentum calculation
  TA_MOM(
    this->startIdx,
    this->endIdx,
    this->inClose,
    optInTimePeriod,
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
