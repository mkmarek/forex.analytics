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

    unsigned tradeCount = trades.size();

    for (unsigned i = 0; i < tradeCount; i++) {

        v8::Local<v8::Object> object = v8::Object::New(isolate);

        const Trade& trade = trades[i];

        Trade::ToObject(trade, object, isolate);

        output->Set(i, object);
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
    const std::vector<std::map<std::string, IndicatorData> >& data) const {
    bool shouldBuy, shouldSell, buy;
    std::vector<Trade> trades;
    const Candlestick * start = nullptr;

    for (unsigned long i = 0; i < data.size(); i++) {
        shouldBuy = chromosome->buy->Evaluate(data[i]);
        shouldSell = chromosome->sell->Evaluate(data[i]);

        if (shouldBuy == true && shouldSell == false) {
            // if we're in trade and we're selling then close it and start a new one
            if (start != nullptr && buy == false) {

                Trade t;
                t.Start = *start;
                t.End = data[i].begin()->second.candlestick;
                t.Buy = buy;

                trades.push_back(t);

                start = &(data[i].begin()->second.candlestick);
            }
            // or if not in trade at all
            else if (start == nullptr) {
                start = &(data[i].begin()->second.candlestick);
            }

            buy = true;
        }

        if (shouldBuy == false && shouldSell == true) {
            if (start != nullptr && buy == true) {

                Trade t;
                t.Start = *start;
                t.End = data[i].begin()->second.candlestick;
                t.Buy = buy;

                trades.push_back(t);

                start = &(data[i].begin()->second.candlestick);

            } else if (start == nullptr) {
                start = &(data[i].begin()->second.candlestick);
            }

            buy = false;
        }
    }

    return trades;
} // TradingSimulator::Simulate
