#include "../../include/indicators/Indicator.h"
#include "../../include/indicators/factories/IndicatorFactory.h"

INDICATOR(Stochastic)(const std::vector<Candlestick>* candlesticks, int* startIndex)
{
  std::vector<double> indicatorData = std::vector<double>();

  int outBeginIdx;
  int outNbElement;
  int optInFastKPeriod = 15;
  int optInSlowKPeriod = 5;
  int optInSlowDPeriod = 3;

  if (candlesticks->size() < optInFastKPeriod ||
      candlesticks->size() < optInSlowDPeriod ||
      candlesticks->size() < optInSlowKPeriod) {

        return indicatorData;
  }

  //Initialize all required parameters
  this->PrepareParameters(candlesticks);

  //Perform the RSI calculation
  TA_STOCH(
    this->startIdx,
    this->endIdx,
    this->inHigh,
    this->inLow,
    this->inClose,
    optInFastKPeriod,
    optInSlowKPeriod,
    TA_MAType::TA_MAType_SMA,
    optInSlowDPeriod,
    TA_MAType::TA_MAType_SMA,
    &outBeginIdx,
    &outNbElement,
    this->outReal, //slow K
    this->outReal2 //slow D
    );


  //Put the result into indicator data vector in RsiModel
  for (int i = 0; i < outNbElement; i++) {
    indicatorData.push_back(outReal[i] - outReal2[i]);
  }

  *startIndex = outBeginIdx;

  //Cleanup all required parameters
  this->Clean();

  return indicatorData;
}