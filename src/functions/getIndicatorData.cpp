#include <nan.h>
#include <node.h>
#include "../../include/TradingSystem.h"
#include "../../include/indicators/Indicator.h"

/**
* Returns an array of objects with indicator values
*/
NAN_METHOD(getIndicatorData)
{
	// expecting two arguments 1. candlestick data 2. genetic algorithm settings
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

	TradingSystem system;
	std::vector<Candlestick> candlesticks;
	v8::Handle<v8::Array> candlestickArray = v8::Handle<v8::Array>::Cast(info[0]);

	Candlestick::CreateFromArray(candlesticks, candlestickArray);

	v8::Handle<v8::Object> configuration = v8::Handle<v8::Object>::Cast(info[1]);

	v8::Handle<v8::Array> indicatorArray = v8::Handle<v8::Array>::Cast(
		configuration->Get(Nan::New<v8::String>("indicators").ToLocalChecked()));
	
	std::vector<BaseIndicator *> indicators = IndicatorFactory::CreateFromArray(indicatorArray);

	v8::Local<v8::Array> output = Nan::New<v8::Array>();

	std::vector<IndicatorTuple> indicatorData =
		system.EvaluateCandlesticks(candlesticks, indicators);

	for (unsigned long i = 0; i < indicatorData.size(); i++)
	{
		v8::Local<v8::Object> obj = Nan::New<v8::Object>();

		typedef std::map<std::string, IndicatorData>::iterator it_type;
		for (it_type iterator = indicatorData[i].Element.begin(); iterator != indicatorData[i].Element.end(); ++iterator)
		{
			obj->Set(Nan::New<v8::String>(iterator->first.c_str()).ToLocalChecked(),
				Nan::New<v8::Number>(iterator->second.data));

			obj->Set(Nan::New<v8::String>("timestamp").ToLocalChecked(),
				Nan::New<v8::Integer>(iterator->second.candlestick.Time));
		}

		output->Set(i, obj);
	}

	info.GetReturnValue().Set(output);
} // getIndicatorData