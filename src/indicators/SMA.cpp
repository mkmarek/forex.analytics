#include "../../include/indicators/Indicator.h"

INDICATOR(SMA)(const std::vector<Candlestick>& candlesticks, int* startIndex)
{
  std::vector<double> indicatorData = std::vector<double>();

  //Initialize all required parameters
  this->PrepareParameters(candlesticks);

  int outBeginIdx;
  int outNbElement;
  int optInTimePeriod = 14;

  //Perform the RSI calculation
  TA_SMA(
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
