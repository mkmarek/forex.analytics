#include "nan.h"

#include "../include/TradingSimulator.h"
#include <iostream>

std::vector<Trade>* TradingSimulator::Simulate(
    const BinaryTreeChromosome * chromosome,
    const std::vector<double>* closePrices,
    const std::vector<std::unordered_map<std::string, double>>* data) const {

    std::vector<Trade>* trades = new std::vector<Trade>();

    int startIndex = -1;
	  int profitIndex = 0;
	  int lossIndex = 0;
    double maximumPotentialLoss = 0.0;
    double maximumPotentialProfit = 0.0;

		bool shouldBuy, shouldSell, buy = false;

		for (unsigned long i = 0; i < data->size(); i++) {
			shouldBuy = chromosome->shouldBuy(data->at(i));
			shouldSell = chromosome->shouldSell(data->at(i));

			if (shouldBuy == true && shouldSell == false) {
				// if we're in trade and we're selling then close it and start a new one
				if (startIndex >= 0 && buy == false) {

					Trade t;
					t.Start = startIndex;
					t.End = i;
          t.StartPrice = closePrices->at(startIndex);
					t.EndPrice = closePrices->at(i);
					t.Buy = buy;
					t.MaximumProfit = maximumPotentialProfit;
					t.MaximumLoss = maximumPotentialLoss;
					t.ProfitBeforeLoss = profitIndex < lossIndex;

					trades->push_back(t);

					startIndex = i;

					profitIndex = 0;
					lossIndex = 0;
					maximumPotentialLoss = 0.0;
					maximumPotentialProfit = 0.0;
				}
				// or if not in trade at all
				else {
					startIndex = i;
				}


				buy = true;
			}

			if (startIndex >= 0 && buy == true)
			{
				double intermediateProfit = closePrices->at(i) - closePrices->at(startIndex);
				double intermediateLoss = closePrices->at(startIndex) - closePrices->at(i);

				if (intermediateLoss > maximumPotentialLoss) {
					maximumPotentialLoss = intermediateLoss;
					lossIndex = i;
				}

				if (intermediateProfit > maximumPotentialProfit) {
					maximumPotentialProfit = intermediateProfit;
					profitIndex = i;
				}
			}

			if (shouldBuy == false && shouldSell == true) {
				if (startIndex >= 0 && buy == true) {

					Trade t;
					t.Start = startIndex;
					t.End = i;
          t.StartPrice = closePrices->at(startIndex);
					t.EndPrice = closePrices->at(i);
					t.Buy = buy;
					t.MaximumProfit = maximumPotentialProfit;
					t.MaximumLoss = maximumPotentialLoss;
					t.ProfitBeforeLoss = profitIndex < lossIndex;

					trades->push_back(t);

					startIndex = i;

					profitIndex = 0;
					lossIndex = 0;
					maximumPotentialLoss = 0.0;
					maximumPotentialProfit = 0.0;

				}
				else {
          startIndex = i;
				}

				buy = false;
			}

			if (buy == false && startIndex >= 0)
			{

				double intermediateProfit = closePrices->at(startIndex) - closePrices->at(i);
				double intermediateLoss = closePrices->at(i) - closePrices->at(startIndex);

				if (intermediateLoss > maximumPotentialLoss) {
					maximumPotentialLoss = intermediateLoss;
					lossIndex = i;
				}

				if (intermediateProfit > maximumPotentialProfit) {
					maximumPotentialProfit = intermediateProfit;
					profitIndex = i;
				}
			}

    }

    return trades;
} // TradingSimulator::Simulate


double Trade::getRevenue() const {
    return this->Buy ?
           this->EndPrice - this->StartPrice :
           this->StartPrice - this->EndPrice;
}

void Trade::ToArray(
    const std::vector<Trade>& trades,
    v8::Local<v8::Array>& output) {

    size_t tradeCount = trades.size();

    for (size_t i = 0; i < tradeCount; i++) {

        v8::Local<v8::Object> object = Nan::New<v8::Object>();

        const Trade& trade = trades[i];

        Trade::ToObject(trade, object);

        output->Set(static_cast<uint32_t>(i), object);
    }
}

void Trade::ToObject(
    const Trade& trade,
    v8::Local<v8::Object>& output) {

    output->Set(Nan::New<v8::String>("Buy").ToLocalChecked(),
                Nan::New<v8::Boolean>(trade.Buy));

    output->Set(Nan::New<v8::String>("Revenue").ToLocalChecked(),
                Nan::New<v8::Number>(trade.getRevenue()));

    output->Set(Nan::New<v8::String>("MaximumLoss").ToLocalChecked(),
                Nan::New<v8::Number>(trade.MaximumLoss));

    output->Set(Nan::New<v8::String>("MaximumProfit").ToLocalChecked(),
                Nan::New<v8::Number>(trade.MaximumProfit));

    output->Set(Nan::New<v8::String>("ProfitBeforeLoss").ToLocalChecked(),
                Nan::New<v8::Number>(trade.ProfitBeforeLoss));

    output->Set(Nan::New<v8::String>("StartPrice").ToLocalChecked(),
                Nan::New<v8::Number>(trade.StartPrice));

    output->Set(Nan::New<v8::String>("EndPrice").ToLocalChecked(),
                Nan::New<v8::Number>(trade.EndPrice));
}
