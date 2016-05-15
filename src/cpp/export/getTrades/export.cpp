#include <nan.h>
#include <node.h>
#include "../../../include/TradingSimulator.h"
#include "../../../include/utils/Utils.h"

NAN_METHOD(getTrades)
{
	// expecting two arguments 1. candlestick data 2. trade settings
	if (info.Length() < 1)
	{
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}

	if (!info[0]->IsObject() || info[0]->IsArray())
	{
		Nan::ThrowTypeError("Wrong first argument. Expecting object with trade configuration");
		return;
	}

	TradingSimulator simulator;
	v8::Local<v8::Array> output = Nan::New<v8::Array>();
	std::vector<double> closePrices;

	v8::Handle<v8::Object> configuration = v8::Handle<v8::Object>::Cast(info[0]);

	v8::Handle<v8::Object> indicators = v8::Handle<v8::Object>::Cast(
		configuration->Get(Nan::New<v8::String>("indicators").ToLocalChecked()));

	std::unordered_map<std::string, std::vector<double>> indicatorValues;
	std::vector<std::string> indicatorTypes;

	PopulateIndicators(indicatorTypes, indicatorValues, indicators);

	v8::Handle<v8::Object> strategy = v8::Handle<v8::Object>::Cast(
		configuration->Get(Nan::New<v8::String>("strategy").ToLocalChecked()));

	std::vector<std::unordered_map<std::string, double>> remappedValues =
		RemapMapArray(&indicatorValues);

	BinaryTreeChromosome* chromosome = BinaryTreeChromosome::FromJs(
		indicatorTypes, strategy);

	std::vector<Trade>* trades = simulator.Simulate(
		chromosome, &closePrices, &remappedValues);

	Trade::ToArray(*trades, output);

	info.GetReturnValue().Set(output);

	delete trades;
	delete chromosome;
} // getTrades
