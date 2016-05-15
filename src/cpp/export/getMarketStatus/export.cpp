#include <nan.h>
#include <node.h>
#include "../../../include/Analytics.h"
#include "../../../include/utils/Utils.h"

NAN_METHOD(getMarketStatus)
{
	// expecting two arguments 1. candlestick data
	if (info.Length() < 1)
	{
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}

	//TODO validate configuration

	v8::Handle<v8::Object> configuration = v8::Handle<v8::Object>::Cast(info[0]);

	v8::Handle<v8::Object> strategy = v8::Handle<v8::Object>::Cast(
		configuration->Get(Nan::New<v8::String>("strategy").ToLocalChecked()));

	v8::Handle<v8::Object> indicators = v8::Handle<v8::Object>::Cast(
		configuration->Get(Nan::New<v8::String>("indicators").ToLocalChecked()));

	std::unordered_map<std::string, std::vector<double>> indicatorValues;
	std::vector<std::string> indicatorTypes;

	PopulateIndicators(indicatorTypes, indicatorValues, indicators);

	BinaryTreeChromosome* chromosome = BinaryTreeChromosome::FromJs(
		indicatorTypes,
		strategy
		);

		std::vector<std::unordered_map<std::string, double>> remappedValues =
			RemapMapArray(&indicatorValues);

	bool shouldBuy = chromosome->shouldBuy(
		remappedValues[remappedValues.size() - 1]);
	bool shouldSell = chromosome->shouldSell(
		remappedValues[remappedValues.size() - 1]);

	v8::Local<v8::Object> output = Nan::New<v8::Object>();

	output->Set(Nan::New<v8::String>("shouldBuy").ToLocalChecked(),
	            Nan::New<v8::Boolean>(shouldBuy));

	output->Set(Nan::New<v8::String>("shouldSell").ToLocalChecked(),
	            Nan::New<v8::Boolean>(shouldSell));

	info.GetReturnValue().Set(output);

	delete chromosome;
} // getTrades
