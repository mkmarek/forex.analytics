#include <nan.h>
#include "../../include/TradingSystem.h"

/**
* Converts OLHC candlesticks from lower timeframe to larger timeframe
*/
NAN_METHOD(convertOHLC)
{

	// expecting two arguments 1. candlestick data 2. genetic algorithm settings
	if (info.Length() < 2)
	{
		// Throw an Error that is passed back to JavaScript
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}

	if (!info[0]->IsArray())
	{
		Nan::ThrowTypeError("Wrong first argument. Expecting array of candlesticks");
		return;
	}

	if (!info[1]->IsInt32())
	{
		Nan::ThrowTypeError("Wrong second argument. Expecting integer value describing timeframe in seconds");
		return;
	}

	TradingSystem system;
	std::vector<Candlestick> candlesticks;
	auto candlestickArray = v8::Handle<v8::Array>::Cast(info[0]);

	Candlestick::CreateFromArray(candlesticks, candlestickArray);
	std::vector<Candlestick> outputCandlesticks = system.ConvertOHLCToLargerTimeframe(candlesticks, info[1]->Int32Value());

	auto output = Nan::New<v8::Array>();
	Candlestick::ToArray(outputCandlesticks, output);

	info.GetReturnValue().Set(output);

} // convertOHLC