#ifndef __INDICATOR_H
#define __INDICATOR_H

#include "../stdafx.h"
#include "../models/Candlestick.h"
#include "./BaseIndicator.h"

#include <iostream>
#include <ctime>



#define INDICATOR(m) \
\
    class m : Indicator<m> \
    { \
public: \
    double Value; \
\
protected: \
    std::vector<double> _GetIndicatorData(const std::vector<Candlestick>* candlesticks, int * startIndex) override; \
    }; \
\
    bool m ## Indicator = IndicatorFactory::Register(#m, &Indicator<m>::Create); \
\
    std::vector<double> m::_GetIndicatorData

template<typename T>
class Indicator : public BaseIndicator
{
public:

double MIN_VALUE;
double MAX_VALUE;

static BaseIndicator * Create() {
    return (BaseIndicator *)(new T());
}

virtual std::vector<double> GetIndicatorData(const std::vector<Candlestick>* candlesticks, int * startIndex) override {

	std::vector<double> v = this->_GetIndicatorData(candlesticks, startIndex);

    for (unsigned long i = 0; i < v.size(); i++) {
        if (this->MIN_VALUE > v[i])
            this->MIN_VALUE = v[i];

        if (this->MAX_VALUE < v[i])
            this->MAX_VALUE = v[i];
    }

	this->MAX_VALUE = this->MAX_VALUE * 1000;
	this->MIN_VALUE = this->MIN_VALUE * 1000;

	this->uniform_dist = std::uniform_real_distribution<double>(this->MIN_VALUE, this->MAX_VALUE);

    return v;
}

protected:
virtual std::vector<double> _GetIndicatorData(const std::vector<Candlestick>* candlesticks, int * startIndex) = 0;

static const bool IsRegistered_;

};

#endif // ifndef __INDICATOR_H
