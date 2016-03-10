#include <nan.h>
#include <node.h>
#include "../../include/TradingSystem.h"
#include "../../include/TradingSimulator.h"
#include "../../include/indicators/Indicator.h"

NAN_METHOD(getTrades)
{
	// expecting two arguments 1. candlestick data 2. trade settings
	if (info.Length() < 2)
	{
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}

	if (!info[0]->IsArray())
	{
		Nan::ThrowTypeError("Wrong first argument. Expecting array of candlesticks");
		return;
	}

	if (!info[1]->IsObject() || info[1]->IsArray())
	{
		Nan::ThrowTypeError("Wrong second argument. Expecting object with trade configuration");
		return;
	}

	TradingSimulator simulator;
	TradingSystem system;
	std::vector<Candlestick> candlesticks;
	v8::Local<v8::Array> output = Nan::New<v8::Array>();

	v8::Handle<v8::Object> configuration = v8::Handle<v8::Object>::Cast(info[1]);
	v8::Handle<v8::Array> candlestickArray = v8::Handle<v8::Array>::Cast(info[0]);

	Candlestick::CreateFromArray(candlesticks, candlestickArray);

	std::vector<BaseIndicator *> indicators = IndicatorFactory::CreateFromArray(v8::Handle<v8::Array>::Cast(
		configuration->Get(Nan::New<v8::String>("indicators").ToLocalChecked())));

	v8::Handle<v8::Object> strategy = v8::Handle<v8::Object>::Cast(
		configuration->Get(Nan::New<v8::String>("strategy").ToLocalChecked()));

	BinaryTreeChromosome* chromosome = BinaryTreeChromosome::FromJs(indicators, strategy);

	std::vector<IndicatorTuple> dataSet = system.EvaluateCandlesticks(candlesticks, indicators);

	std::vector<Trade>* trades = simulator.Simulate(chromosome, &dataSet);

	Trade::ToArray(*trades, output);

	info.GetReturnValue().Set(output);

	delete trades;
	delete chromosome;
} // getTrades