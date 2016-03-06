#include "../../include/indicators/Indicator.h"

INDICATOR(CCI)(const std::vector<Candlestick>& candlesticks, int* startIndex)
{
  std::vector<double> indicatorData = std::vector<double>();

  int outBeginIdx;
  int outNbElement;
  int optInTimePeriod = 14;

  if (candlesticks.size() < optInTimePeriod) {
    return indicatorData;
  }

  //Initialize all required parameters
  this->PrepareParameters(candlesticks);

  //Perform the CCI calculation
  TA_CCI(
    this->startIdx,
    this->endIdx,
    this->inHigh,
    this->inLow,
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
