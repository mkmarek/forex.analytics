#ifndef __INDICATOR_H
#define __INDICATOR_H

#include "../stdafx.h"
#include "../models/Candlestick.h"
#include "./BaseIndicator.h"
#include "./factories/IndicatorFactory.h"

#include <iostream>

#define INDICATOR(m) \
\
    class m : Indicator<m> \
    { \
public: \
    double Value; \
\
protected: \
    std::vector<double> _GetIndicatorData(const std::vector<Candlestick>&candlesticks, int * startIndex) override; \
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

virtual std::vector<double> GetIndicatorData(const std::vector<Candlestick>& candlesticks, int * startIndex) override {
    std::vector<double> v = this->_GetIndicatorData(candlesticks, startIndex);

    for (unsigned long i = 0; i < v.size(); i++) {
        if (this->MIN_VALUE > v[i])
            this->MIN_VALUE = v[i];

        if (this->MAX_VALUE < v[i])
            this->MAX_VALUE = v[i];
    }

    return v;
}

virtual double getRandomValue() const override {
    return this->fRand(this->MIN_VALUE, this->MAX_VALUE);
}

virtual bool Evaluate(Sign sign, double value, const IndicatorData& data) const override {
    switch (sign) {
        case Sign::Gt:
            return value > data.data;
        case Sign::Lt:
            return value < data.data;
    }

    return false;
}

protected:
virtual std::vector<double> _GetIndicatorData(const std::vector<Candlestick>& candlesticks, int * startIndex) = 0;

static const bool IsRegistered_;

double fRand(double fMin, double fMax) const {
    double f = (double)rand() / RAND_MAX;

    return fMin + f * (fMax - fMin);
}
};

#endif // ifndef __INDICATOR_H
