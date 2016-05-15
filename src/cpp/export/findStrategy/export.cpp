#include <nan.h>
#include <node.h>

#include "../../../include/export/findStrategy/FindStrategyBaton.h"
#include "../../../include/export/findStrategy/FindStrategyAsyncWorker.h"
#include "../../../include/export/findStrategy/StrategyUpdateBaton.h"
#include "../../../include/Analytics.h"
#include "../../../include/utils/Utils.h"

bool findStrategyValidateInput(const Nan::FunctionCallbackInfo<v8::Value>& args);
bool validateConfiguration(v8::Handle<v8::Object> configuration);


NAN_METHOD(findStrategy)
{
	if (findStrategyValidateInput(info))
	{
		std::unordered_map<std::string, std::vector<double>> input;
		std::vector<std::string> inputTypes;

		v8::Handle<v8::Object> configuration = getObjectFromArguments(info, 0);

		FindStrategyBaton* baton = new FindStrategyBaton();
		baton->PopulateFrom(configuration);

		if (info.Length() > 1 && !info[1]->IsUndefined())
	  {
	    baton->progress = new Nan::Callback(info[2].As<v8::Function>());
	  }

		Nan::Callback* callback = nullptr;
	  if (info.Length() > 2 && !info[2]->IsUndefined())
	  {
	  	callback = new Nan::Callback(info[2].As<v8::Function>());
	  }

		auto worker = new FindStrategyAsyncWorker(callback, baton);
		Nan::AsyncQueueWorker(worker);
	}

	info.GetReturnValue().Set(Nan::Undefined());
} // findStrategy


bool findStrategyValidateInput(const Nan::FunctionCallbackInfo<v8::Value>& args)
{
	if (args.Length() < 2)
	{
		Nan::ThrowTypeError("Wrong number of arguments");
		return false;
	}

	if (!args[0]->IsArray())
	{
		Nan::ThrowTypeError("Wrong first argument. Expecting array of close values");
		return false;
	}

	if (args[2]->IsUndefined() || !args[2]->IsObject() || args[2]->IsArray())
	{
		Nan::ThrowTypeError("Wrong third argument. Expecting object with genetic algorithm configuration");
		return false;
	}

	if (!args[3]->IsUndefined() && (!args[3]->IsFunction()))
	{
		Nan::ThrowTypeError("Wrong fourth argument. Expecting a function");
		return false;
	}

	if (!args[4]->IsUndefined() && (!args[4]->IsFunction()))
	{
		Nan::ThrowTypeError("Wrong fifth argument. Expecting a function");
		return false;
	}

	if (!validateConfiguration(getObjectFromArguments(args, 2))) {
		return false;
	}

	return true;
}

bool validateConfiguration(v8::Handle<v8::Object> configuration) {

	if (!configuration->Has(Nan::New<v8::String>("pipInDecimals").ToLocalChecked()))
	{
		Nan::ThrowTypeError("Expecting configuration object to have pipInDecimals value set");
		return false;
	}

	if (!configuration->Get(Nan::New<v8::String>("pipInDecimals").ToLocalChecked())->IsNumber())
	{
		Nan::ThrowTypeError("Expecting pipInDecimals to be a number");
		return false;
	}

	if (getNumberOrDefault(configuration, "pipInDecimals", 0) <= 0)
	{
		Nan::ThrowTypeError("Expecting pipInDecimals to be larger than 0");
		return false;
	}

	return true;
}
