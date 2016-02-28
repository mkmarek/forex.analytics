#include "../../include/indicators/Indicator.h"

INDICATOR(Stochastic_D)(const std::vector<Candlestick>& candlesticks, int* startIndex)
{
  std::vector<double> indicatorData = std::vector<double>();

  //Initialize all required parameters
  this->PrepareParameters(candlesticks);

  int outBeginIdx;
  int outNbElement;
  int optInFastKPeriod = 15;
  int optInSlowKPeriod = 5;
  int optInSlowDPeriod = 3;

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
    indicatorData.push_back(outReal2[i]);
  }

  *startIndex = outBeginIdx;

  //Cleanup all required parameters
  this->Clean();

  return indicatorData;
}

INDICATOR(Stochastic_K)(const std::vector<Candlestick>& candlesticks, int* startIndex)
{
  std::vector<double> indicatorData = std::vector<double>();

  //Initialize all required parameters
  this->PrepareParameters(candlesticks);

  int outBeginIdx;
  int outNbElement;
  int optInFastKPeriod = 15;
  int optInSlowKPeriod = 5;
  int optInSlowDPeriod = 3;

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
    indicatorData.push_back(outReal[i]);
  }

  *startIndex = outBeginIdx;

  //Cleanup all required parameters
  this->Clean();

  return indicatorData;
}
