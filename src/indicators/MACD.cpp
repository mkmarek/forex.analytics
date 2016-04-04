#include "../../include/indicators/Indicator.h"
#include "../../include/indicators/factories/IndicatorFactory.h"

INDICATOR(MACD)(const std::vector<Candlestick>* candlesticks, int* startIndex)
{
  std::vector<double> indicatorData = std::vector<double>();

  int outBeginIdx;
  int outNbElement;
  int optInFastPeriod = 12;
  int optInSlowPeriod = 26;
  int optInSignalPeriod = 9;

  if (candlesticks->size() < optInFastPeriod ||
      candlesticks->size() < optInSlowPeriod ||
      candlesticks->size() < optInSignalPeriod) {

        return indicatorData;
  }

  //Initialize all required parameters
  this->PrepareParameters(candlesticks);

  //Perform the RSI calculation
  TA_MACD(
    this->startIdx,
    this->endIdx,
    this->inClose,
    optInFastPeriod,
    optInSlowPeriod,
    optInSignalPeriod,
    &outBeginIdx,
    &outNbElement,
    this->outReal, //MACD
    this->outReal2, //MACD signal
    this->outReal3  //MACD histogram
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

INDICATOR(MACD_Signal)(const std::vector<Candlestick>* candlesticks, int* startIndex)
{
  std::vector<double> indicatorData = std::vector<double>();

  int outBeginIdx;
  int outNbElement;
  int optInFastPeriod = 12;
  int optInSlowPeriod = 26;
  int optInSignalPeriod = 9;

  if (candlesticks->size() < optInFastPeriod ||
      candlesticks->size() < optInSlowPeriod ||
      candlesticks->size() < optInSignalPeriod) {

        return indicatorData;
  }

  //Initialize all required parameters
  this->PrepareParameters(candlesticks);

  //Perform the RSI calculation
  TA_MACD(
    this->startIdx,
    this->endIdx,
    this->inClose,
    optInFastPeriod,
    optInSlowPeriod,
    optInSignalPeriod,
    &outBeginIdx,
    &outNbElement,
    this->outReal, //MACD
    this->outReal2, //MACD signal
    this->outReal3  //MACD histogram
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

INDICATOR(MACD_Histogram)(const std::vector<Candlestick>* candlesticks, int* startIndex)
{
  std::vector<double> indicatorData = std::vector<double>();


  int outBeginIdx;
  int outNbElement;
  int optInFastPeriod = 12;
  int optInSlowPeriod = 26;
  int optInSignalPeriod = 9;

  if (candlesticks->size() < optInFastPeriod ||
      candlesticks->size() < optInSlowPeriod ||
      candlesticks->size() < optInSignalPeriod) {

        return indicatorData;
  }

  //Initialize all required parameters
  this->PrepareParameters(candlesticks);

  //Perform the RSI calculation
  TA_MACD(
    this->startIdx,
    this->endIdx,
    this->inClose,
    optInFastPeriod,
    optInSlowPeriod,
    optInSignalPeriod,
    &outBeginIdx,
    &outNbElement,
    this->outReal, //MACD
    this->outReal2, //MACD signal
    this->outReal3  //MACD histogram
    );

  //Put the result into indicator data vector in RsiModel
  for (int i = 0; i < outNbElement; i++) {
    indicatorData.push_back(outReal3[i]);
  }

  *startIndex = outBeginIdx;

  //Cleanup all required parameters
  this->Clean();

  return indicatorData;
}
