#include "../include/TradingSimulator.h"

double Trade::getRevenue() const {
    return this->Buy ?
           this->End.Close - this->Start.Close :
           this->Start.Close - this->End.Close;
}

void Trade::ToArray(
    const std::vector<Trade>& trades,
    v8::Local<v8::Array>& output,
    v8::Isolate * isolate) {

    size_t tradeCount = trades.size();

    for (size_t i = 0; i < tradeCount; i++) {

        v8::Local<v8::Object> object = v8::Object::New(isolate);

        const Trade& trade = trades[i];

        Trade::ToObject(trade, object, isolate);

        output->Set(static_cast<uint32_t>(i), object);
    }
}

void Trade::ToObject(
    const Trade& trade,
    v8::Local<v8::Object>& output,
    v8::Isolate * isolate) {

    output->Set(v8::String::NewFromUtf8(isolate, "Buy"),
                v8::Boolean::New(isolate, trade.Buy));

    output->Set(v8::String::NewFromUtf8(isolate, "Revenue"),
                v8::Number::New(isolate, trade.getRevenue()));

    output->Set(v8::String::NewFromUtf8(isolate, "MaximumLoss"),
                v8::Number::New(isolate, trade.MaximumLoss));

    output->Set(v8::String::NewFromUtf8(isolate, "MaximumProffit"),
                v8::Number::New(isolate, trade.MaximumProffit));

    v8::Local<v8::Object> start = v8::Object::New(isolate);
    v8::Local<v8::Object> end = v8::Object::New(isolate);

    Candlestick::ToObject(trade.Start, start, isolate);
    Candlestick::ToObject(trade.End, end, isolate);

    output->Set(v8::String::NewFromUtf8(isolate, "start"),
                start);
    output->Set(v8::String::NewFromUtf8(isolate, "end"),
                end);

}

std::vector<Trade> TradingSimulator::Simulate(
    const BinaryTreeChromosome * chromosome,
    const std::vector<IndicatorTuple>& data) const {
    bool shouldBuy, shouldSell, buy = false;
    std::vector<Trade> trades;

    const Candlestick * start = nullptr;
    double maximumPotentialLoss = 0.0;
    double maximumPotentialProffit = 0.0;

    for (unsigned long i = 0; i < data.size(); i++) {
        shouldBuy = chromosome->buy->Evaluate(data[i].Element);
        shouldSell = chromosome->sell->Evaluate(data[i].Element);

        if (shouldBuy == true && shouldSell == false) {
            // if we're in trade and we're selling then close it and start a new one
            if (start != nullptr && buy == false) {

                Trade t;
                t.Start = *start;
                t.End = data[i].Element.begin()->second.candlestick;
                t.Buy = buy;
                t.MaximumProffit = maximumPotentialProffit;
                t.MaximumLoss = maximumPotentialLoss;

                trades.push_back(t);

                start = &(data[i].Element.begin()->second.candlestick);

                maximumPotentialLoss = 0.0;
                maximumPotentialProffit = 0.0;
            }
            // or if not in trade at all
            else if (start == nullptr) {
                start = &(data[i].Element.begin()->second.candlestick);
            }

            double intermediateProffit = data[i].Element.begin()->second.candlestick.Close - start->Close;
            double intermediateLoss = start->Close - data[i].Element.begin()->second.candlestick.Close;

            if (intermediateLoss > maximumPotentialLoss)
              maximumPotentialLoss = intermediateLoss;

            if (intermediateProffit > maximumPotentialProffit)
              maximumPotentialProffit = intermediateProffit;

            buy = true;
        }

        if (shouldBuy == false && shouldSell == true) {
            if (start != nullptr && buy == true) {

                Trade t;
                t.Start = *start;
                t.End = data[i].Element.begin()->second.candlestick;
                t.Buy = buy;
                t.MaximumProffit = maximumPotentialProffit;
                t.MaximumLoss = maximumPotentialLoss;

                trades.push_back(t);

                start = &(data[i].Element.begin()->second.candlestick);

                maximumPotentialLoss = 0.0;
                maximumPotentialProffit = 0.0;

            } else if (start == nullptr) {
                start = &(data[i].Element.begin()->second.candlestick);
            }

            double intermediateProffit = start->Close - data[i].Element.begin()->second.candlestick.Close;
            double intermediateLoss = data[i].Element.begin()->second.candlestick.Close - start->Close;

            if (intermediateLoss > maximumPotentialLoss)
              maximumPotentialLoss = intermediateLoss;

            if (intermediateProffit > maximumPotentialProffit)
              maximumPotentialProffit = intermediateProffit;

            buy = false;
        }
    }

    return trades;
} // TradingSimulator::Simulate
