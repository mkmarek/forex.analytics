// analytics.cpp
#include <node.h>
#include "../include/TradingSystem.h"
#include "../include/TradingSimulator.h"
#include "../include/indicators/Indicator.h"

namespace analytics {

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Array;
	using v8::Object;
	using v8::Integer;
	using v8::String;
	using v8::Value;
	using v8::Exception;
	using v8::Handle;
	using v8::Boolean;

	void findStrategy(const FunctionCallbackInfo<Value>& args) {
		Isolate * isolate = args.GetIsolate();

		// expecting two arguments 1. candlestick data 2. genetic algorithm settings
		if (args.Length() < 2) {
			// Throw an Error that is passed back to JavaScript
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}

		if (!args[0]->IsArray()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong first argument. Expecting array of candlesticks")));
			return;
		}

		if (!args[1]->IsObject() || args[1]->IsArray()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong second argument. Expecting object with genetic algorithm configuration")));
			return;
		}

		Handle<Object> configuration = Handle<Object>::Cast(args[1]);
		Handle<Array> candlestickArray = Handle<Array>::Cast(args[0]);

		TradingSystem system;

		std::vector<Candlestick> candlesticks;

		Candlestick::CreateFromArray(candlesticks, candlestickArray, isolate);

		int populationCount = configuration->Get(v8::String::NewFromUtf8(isolate, "populationCount"))->Int32Value();
		int generationCount = configuration->Get(v8::String::NewFromUtf8(isolate, "generationCount"))->Int32Value();
		int selectionAmount = configuration->Get(v8::String::NewFromUtf8(isolate, "selectionAmount"))->Int32Value();

		double leafValueMutationProbability = configuration->Get(v8::String::NewFromUtf8(isolate, "leafValueMutationProbability"))->NumberValue();
		double leafSignMutationProbability = configuration->Get(v8::String::NewFromUtf8(isolate, "leafSignMutationProbability"))->NumberValue();
		double logicalNodeMutationProbability = configuration->Get(v8::String::NewFromUtf8(isolate, "logicalNodeMutationProbability"))->NumberValue();
		double leafIndicatorMutationProbability = configuration->Get(v8::String::NewFromUtf8(isolate, "leafIndicatorMutationProbability"))->NumberValue();
		double crossoverProbability = configuration->Get(v8::String::NewFromUtf8(isolate, "crossoverProbability"))->NumberValue();
		double fitness = 0;

		Handle<Array> indicatorArray = Handle<Array>::Cast(configuration->Get(v8::String::NewFromUtf8(isolate, "indicators")));
		unsigned indicatorCount = indicatorArray->Length();

		std::vector<std::string> indicatorNames;

		for (unsigned i = 0; i < indicatorCount; i++) {
			indicatorNames.push_back(std::string(*v8::String::Utf8Value(indicatorArray->Get(i)->ToString())));
		}

		std::vector<BaseIndicator *> indicators;

		for (unsigned long i = 0; i < indicatorNames.size(); i++) {
			BaseIndicator * indicator = IndicatorFactory::Create(indicatorNames[i]);
			indicators.push_back(indicator);
		}

		BinaryTreeChromosome * chromosome = system.PerformAnalysis(
			candlesticks,
			indicators,
			populationCount,
			generationCount,
			selectionAmount,
			leafValueMutationProbability,
			leafSignMutationProbability,
			logicalNodeMutationProbability,
			leafIndicatorMutationProbability,
			crossoverProbability,
			&fitness);

		Local<Object> buy = Object::New(isolate);
		Local<Object> sell = Object::New(isolate);

		chromosome->buy->ToJs(buy, isolate);
		chromosome->sell->ToJs(sell, isolate);

		Local<Object> strategy = Object::New(isolate);
		strategy->Set(v8::String::NewFromUtf8(isolate, "buy"), buy);
		strategy->Set(v8::String::NewFromUtf8(isolate, "sell"), sell);

		args.GetReturnValue().Set(strategy);

		delete chromosome;
	} // findStrategy

	/**
	 * Converts OLHC candlesticks from lower timeframe to larger timeframe
	 */
	void convertOHLC(const FunctionCallbackInfo<Value>& args) {
		Isolate * isolate = args.GetIsolate();

		// expecting two arguments 1. candlestick data 2. genetic algorithm settings
		if (args.Length() < 2) {
			// Throw an Error that is passed back to JavaScript
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}

		if (!args[0]->IsArray()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong first argument. Expecting array of candlesticks")));
			return;
		}

		if (!args[1]->IsInt32()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong first argument. Expecting integer value describing timeframe in seconds")));
			return;
		}

		TradingSystem system;
		std::vector<Candlestick> candlesticks;
		Handle<Array> candlestickArray = Handle<Array>::Cast(args[0]);

		Candlestick::CreateFromArray(candlesticks, candlestickArray, isolate);

		std::vector<Candlestick> outputCandlesticks = system.ConvertOHLCToLargerTimeframe(candlesticks, args[1]->Int32Value());

		Local<Array> output = Array::New(isolate);

		Candlestick::ToArray(outputCandlesticks, output, isolate);

		args.GetReturnValue().Set(output);

	} // convertOHLC

	void getTrades(const FunctionCallbackInfo<Value>& args) {
		Isolate * isolate = args.GetIsolate();

		// expecting two arguments 1. candlestick data 2. trade settings
		if (args.Length() < 2) {
			// Throw an Error that is passed back to JavaScript
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}

		if (!args[0]->IsArray()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong first argument. Expecting array of candlesticks")));
			return;
		}

		if (!args[1]->IsObject() || args[1]->IsArray()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong second argument. Expecting object with trade configuration")));
			return;
		}

		Handle<Object> configuration = Handle<Object>::Cast(args[1]);
		Handle<Array> candlestickArray = Handle<Array>::Cast(args[0]);

		std::vector<Candlestick> candlesticks;

		Candlestick::CreateFromArray(candlesticks, candlestickArray, isolate);

		Handle<Array> indicatorArray = Handle<Array>::Cast(
			configuration->Get(v8::String::NewFromUtf8(isolate, "indicators")));
		unsigned indicatorCount = indicatorArray->Length();

		std::vector<std::string> indicatorNames;

		for (unsigned i = 0; i < indicatorCount; i++) {
			indicatorNames.push_back(std::string(*v8::String::Utf8Value(indicatorArray->Get(i)->ToString())));
		}

		std::vector<BaseIndicator *> indicators;

		for (unsigned long i = 0; i < indicatorNames.size(); i++) {
			BaseIndicator * indicator = IndicatorFactory::Create(indicatorNames[i]);
			indicators.push_back(indicator);
		}

		Handle<Object> strategy = Handle<Object>::Cast(
			configuration->Get(v8::String::NewFromUtf8(isolate, "strategy")));

		TradingSimulator simulator;
		TradingSystem system;

		std::vector<IndicatorTuple> dataSet = system
			.EvaluateCandlesticks(candlesticks, indicators);

		BinaryTreeChromosome * chromosome = BinaryTreeChromosome::FromJs(
			indicators,
			strategy,
			isolate
			);

		std::vector<Trade> trades = simulator.Simulate(chromosome, dataSet);

		Local<Array> output = Array::New(isolate);

		Trade::ToArray(trades, output, isolate);

		args.GetReturnValue().Set(output);

		delete chromosome;
	} // getTrades

	void getMarketStatus(const FunctionCallbackInfo<Value>& args) {
		Isolate * isolate = args.GetIsolate();

		// expecting two arguments 1. candlestick data 2. trade settings
		if (args.Length() < 2) {
			// Throw an Error that is passed back to JavaScript
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}

		if (!args[0]->IsArray()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong first argument. Expecting array of candlesticks")));
			return;
		}

		if (!args[1]->IsObject() || args[1]->IsArray()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong second argument. Expecting object with trade configuration")));
			return;
		}

		Handle<Object> configuration = Handle<Object>::Cast(args[1]);
		Handle<Array> candlestickArray = Handle<Array>::Cast(args[0]);

		std::vector<Candlestick> candlesticks;

		Candlestick::CreateFromArray(candlesticks, candlestickArray, isolate);

		Handle<Array> indicatorArray = Handle<Array>::Cast(
			configuration->Get(v8::String::NewFromUtf8(isolate, "indicators")));
		unsigned indicatorCount = indicatorArray->Length();

		std::vector<std::string> indicatorNames;

		for (unsigned i = 0; i < indicatorCount; i++) {
			indicatorNames.push_back(std::string(*v8::String::Utf8Value(indicatorArray->Get(i)->ToString())));
		}

		std::vector<BaseIndicator *> indicators;

		for (unsigned long i = 0; i < indicatorNames.size(); i++) {
			BaseIndicator * indicator = IndicatorFactory::Create(indicatorNames[i]);
			indicators.push_back(indicator);
		}

		Handle<Object> strategy = Handle<Object>::Cast(
			configuration->Get(v8::String::NewFromUtf8(isolate, "strategy")));

		TradingSystem system;

		std::vector<IndicatorTuple> dataSet = system
			.EvaluateCandlesticks(candlesticks, indicators);

		BinaryTreeChromosome * chromosome = BinaryTreeChromosome::FromJs(
			indicators,
			strategy,
			isolate
			);

		bool shouldBuy = chromosome->buy->Evaluate(dataSet[dataSet.size() - 2].Element);
		bool shouldSell = chromosome->sell->Evaluate(dataSet[dataSet.size() - 2].Element);

		Local<Object> output = Object::New(isolate);
		output->Set(v8::String::NewFromUtf8(isolate, "shouldBuy"),
			Boolean::New(isolate, shouldBuy));

		output->Set(v8::String::NewFromUtf8(isolate, "shouldSell"),
			Boolean::New(isolate, shouldSell));

		args.GetReturnValue().Set(output);

		delete chromosome;
	} // getTrades

	/**
	 * Returns an array of objects with indicator values
	 */
	void getIndicatorData(const FunctionCallbackInfo<Value>& args) {
		Isolate * isolate = args.GetIsolate();

		// expecting two arguments 1. candlestick data 2. genetic algorithm settings
		if (args.Length() < 2) {
			// Throw an Error that is passed back to JavaScript
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}

		if (!args[0]->IsArray()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong first argument. Expecting array of candlesticks")));
			return;
		}

		TradingSystem system;
		std::vector<Candlestick> candlesticks;
		Handle<Array> candlestickArray = Handle<Array>::Cast(args[0]);

		Candlestick::CreateFromArray(candlesticks, candlestickArray, isolate);

		Handle<Object> configuration = Handle<Object>::Cast(args[1]);

		Handle<Array> indicatorArray = Handle<Array>::Cast(
			configuration->Get(v8::String::NewFromUtf8(isolate, "indicators")));
		unsigned indicatorCount = indicatorArray->Length();

		std::vector<std::string> indicatorNames;

		for (unsigned i = 0; i < indicatorCount; i++) {
			indicatorNames.push_back(std::string(*v8::String::Utf8Value(indicatorArray->Get(i)->ToString())));
		}

		std::vector<BaseIndicator *> indicators;

		for (unsigned long i = 0; i < indicatorNames.size(); i++) {
			BaseIndicator * indicator = IndicatorFactory::Create(indicatorNames[i]);
			indicators.push_back(indicator);
		}

		Local<Array> output = Array::New(isolate);

		std::vector<IndicatorTuple> indicatorData =
			system.EvaluateCandlesticks(candlesticks, indicators);

		for (unsigned long i = 0; i < indicatorData.size(); i++) {

			Local<Object> obj = Object::New(isolate);

			typedef std::map<std::string, IndicatorData>::iterator it_type;
			for (it_type iterator = indicatorData[i].Element.begin(); iterator != indicatorData[i].Element.end(); ++iterator) {

				obj->Set(v8::String::NewFromUtf8(isolate, iterator->first.c_str()),
					v8::Number::New(isolate, iterator->second.data));

				obj->Set(v8::String::NewFromUtf8(isolate, "timestamp"),
					v8::Integer::New(isolate, iterator->second.candlestick.Time));
			}

			output->Set(i, obj);
		}

		args.GetReturnValue().Set(output);

	} // getIndicatorData

	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "findStrategy", findStrategy);
		NODE_SET_METHOD(exports, "convertOHLC", convertOHLC);
		NODE_SET_METHOD(exports, "getTrades", getTrades);
		NODE_SET_METHOD(exports, "getIndicatorData", getIndicatorData);
		NODE_SET_METHOD(exports, "getMarketStatus", getMarketStatus);
	}

	NODE_MODULE(addon, init)

}  // namespace analytics
