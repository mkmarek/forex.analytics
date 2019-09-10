#include <nan.h>
#include <node.h>
#include "../../include/TradingSystem.h"
#include "../../include/indicators/Indicator.h"
#include "../../include/indicators/factories/IndicatorFactory.h"

NAN_METHOD(getMarketStatus)
{
	// expecting two arguments 1. candlestick data
	if (info.Length() < 1)
	{
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}

	if (!info[0]->IsArray())
	{
		Nan::ThrowTypeError("Wrong first argument. Expecting array of candlesticks");
		return;
	}

	v8::Local<v8::Object> configuration = v8::Local<v8::Object>::Cast(info[1]);
	v8::Local<v8::Array> candlestickArray = v8::Local<v8::Array>::Cast(info[0]);

	std::vector<Candlestick> candlesticks;

	Candlestick::CreateFromArray(candlesticks, candlestickArray);

	std::vector<BaseIndicator *> indicators = IndicatorFactory::CreateAll();

	v8::Local<v8::Object> strategy = v8::Local<v8::Object>::Cast(
		configuration->Get(Nan::New<v8::String>("strategy").ToLocalChecked()));

	TradingSystem system;

	const std::vector<IndicatorTuple> dataSet = system
		.EvaluateCandlesticks(candlesticks, indicators);

	BinaryTreeChromosome* chromosome = BinaryTreeChromosome::FromJs(
		indicators,
		strategy
		);

	bool shouldBuy = chromosome->shouldBuy(dataSet[dataSet.size() - 1].Element);
	bool shouldSell = chromosome->shouldSell(dataSet[dataSet.size() - 1].Element);

	v8::Local<v8::Object> output = Nan::New<v8::Object>();

	output->Set(Nan::New<v8::String>("shouldBuy").ToLocalChecked(),
	            Nan::New<v8::Boolean>(shouldBuy));

	output->Set(Nan::New<v8::String>("shouldSell").ToLocalChecked(),
	            Nan::New<v8::Boolean>(shouldSell));

	info.GetReturnValue().Set(output);

	delete chromosome;
} // getTrades
