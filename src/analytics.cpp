// analytics.cpp
#include <nan.h>
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
	using v8::Int32;

	// the 'baton' is the carrier for data between functions
	struct FindStrategyBaton
	{
		// required
		uv_work_s request;
		Nan::Persistent<v8::Promise::Resolver, Nan::CopyablePersistentTraits<v8::Promise::Resolver>>* promiseResolver;
		Nan::Persistent<v8::Function, Nan::CopyablePersistentTraits<v8::Function>>* callback;

		BinaryTreeChromosome * chromosome;
		std::vector<Candlestick> candlesticks;
		std::vector<BaseIndicator*> indicators;
		unsigned populationCount;
		unsigned generationCount;
		unsigned selectionAmount;
		double leafValueMutationProbability;
		double leafSignMutationProbability;
		double logicalNodeMutationProbability;
		double leafIndicatorMutationProbability;
		double crossoverProbability;
	};

	struct StrategyUpdateBaton
	{
		// required
		uv_async_t request;
		Nan::Persistent<v8::Function, Nan::CopyablePersistentTraits<v8::Function>>* callback;

		BinaryTreeChromosome * chromosome;
		double fitness;
		int generation;
	};

	template<typename T>
	static void chromosomeToObject(T* baton, Local<Object>& strategy, Isolate* isolate)
	{
		Local<Object> buy = Nan::New<Object>();
		Local<Object> sell = Nan::New<Object>();

		baton->chromosome->buy->ToJs(buy, isolate);
		baton->chromosome->sell->ToJs(sell, isolate);

		strategy = Nan::New<Object>();
		strategy->Set(v8::String::NewFromUtf8(isolate, "buy"), buy);
		strategy->Set(v8::String::NewFromUtf8(isolate, "sell"), sell);
	}

	void updateStrategyStatusAsync(uv_async_t* handle)
	{
		Nan::HandleScope scope;

		Isolate* isolate = Isolate::GetCurrent();

		StrategyUpdateBaton *baton = static_cast<StrategyUpdateBaton *>(handle->data);

		Local<Object> strategy;

		chromosomeToObject(baton, strategy, isolate);

		Handle<Value> argv[] =
		{
			strategy,
			Handle<Value>(Nan::New<v8::Number>(baton->fitness)),
			Handle<Value>(Nan::New<v8::Int32>(baton->generation)),
		};

		baton->callback->Get(Isolate::GetCurrent())
			->Call(v8::Undefined(Isolate::GetCurrent()), 3, argv);

		uv_close(reinterpret_cast<uv_handle_t*>(handle),
			[](uv_handle_t* h) -> void {delete h; });
	};


	// called by libuv worker in separate thread
	static void findStrategyAsync(uv_work_t *req)
	{
		FindStrategyBaton *baton = static_cast<FindStrategyBaton *>(req->data);

		TradingSystem system;

		auto update = [baton, req](double fitness, BinaryTreeChromosome * chromosome, int generation)
		{
			baton->chromosome = chromosome;

			StrategyUpdateBaton *updateBaton = new StrategyUpdateBaton;
			updateBaton->fitness = fitness;
			updateBaton->chromosome = chromosome;
			updateBaton->generation = generation;
			updateBaton->callback = baton->callback;
			updateBaton->request.data = updateBaton;

			uv_async_init(uv_default_loop(), &updateBaton->request, &updateStrategyStatusAsync);
			uv_async_send(&updateBaton->request);
		};

		baton->chromosome = system.PerformAnalysis(
			baton->candlesticks,
			baton->indicators,
			baton->populationCount,
			baton->generationCount,
			baton->selectionAmount,
			baton->leafValueMutationProbability,
			baton->leafSignMutationProbability,
			baton->logicalNodeMutationProbability,
			baton->leafIndicatorMutationProbability,
			baton->crossoverProbability,
			update);
	}

	// called by libuv in event loop when async function completes
	static void findStrategyAsyncAfter(uv_work_t *req, int status)
	{
		Nan::HandleScope scope;

		Isolate* isolate = Isolate::GetCurrent();

		// get the reference to the baton from the request
		FindStrategyBaton *baton = static_cast<FindStrategyBaton *>(req->data);

		Local<Object> strategy;

		chromosomeToObject(baton, strategy, isolate);

		baton->promiseResolver->Get(Isolate::GetCurrent())->Resolve(strategy);

		baton->promiseResolver->Reset();
		baton->callback->Reset();

		delete baton->promiseResolver;
		delete baton->callback;
		delete baton->chromosome;
		delete baton;
	}

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

		Local<v8::Promise::Resolver> resolver = v8::Promise::Resolver::New(isolate);
		Local<v8::Function> callback = Local<v8::Function>::Cast(args[2]);

		FindStrategyBaton *baton = new FindStrategyBaton;
		baton->request.data = baton;

		baton->promiseResolver = new Nan::Persistent<v8::Promise::Resolver, Nan::CopyablePersistentTraits<v8::Promise::Resolver>>(resolver);
		baton->callback = new Nan::Persistent<v8::Function, Nan::CopyablePersistentTraits<v8::Function>>(callback);
		baton->candlesticks = candlesticks;
		baton->indicators = indicators;
		baton->populationCount = populationCount;
		baton->generationCount = generationCount;
		baton->selectionAmount = selectionAmount;
		baton->leafValueMutationProbability = leafValueMutationProbability;
		baton->leafSignMutationProbability = leafSignMutationProbability;
		baton->logicalNodeMutationProbability = logicalNodeMutationProbability;
		baton->leafIndicatorMutationProbability = leafIndicatorMutationProbability;
		baton->crossoverProbability = crossoverProbability;

		// queue the async function to the event loop
		// the uv default loop is the node.js event loop
		uv_queue_work(uv_default_loop(), &(baton->request), findStrategyAsync, findStrategyAsyncAfter);

		args.GetReturnValue().Set(resolver->GetPromise());
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

		bool shouldBuy = chromosome->buy->Evaluate(dataSet[dataSet.size() - 1].Element);
		bool shouldSell = chromosome->sell->Evaluate(dataSet[dataSet.size() - 1].Element);

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
