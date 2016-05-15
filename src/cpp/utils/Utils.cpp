#include "../../include/utils/Utils.h"

int getIntOrDefault(v8::Handle<v8::Object> object, const char* name, int def)
{
	if (!object->Has(Nan::New<v8::String>(name).ToLocalChecked()))
		return def;

	return object->Get(Nan::New<v8::String>(name).ToLocalChecked())->Int32Value();
}

double getNumberOrDefault(v8::Handle<v8::Object> object, const char* name, double def)
{
	if (!object->Has(Nan::New<v8::String>(name).ToLocalChecked()))
		return def;

	return object->Get(Nan::New<v8::String>(name).ToLocalChecked())->NumberValue();
}

v8::Handle<v8::Object> getObjectFromArguments(const Nan::FunctionCallbackInfo<v8::Value>& args, int index)
{
	if (args.Length() - 1 >= index && !args[index]->IsUndefined())
		return v8::Handle<v8::Object>::Cast(args[index]);

	return Nan::New<v8::Object>();
}

v8::Handle<v8::Array> getArrayFromArguments(const Nan::FunctionCallbackInfo<v8::Value>& args, int index)
{
	if (args.Length() - 1 >= index && !args[index]->IsUndefined())
		return v8::Handle<v8::Array>::Cast(args[index]);

	return Nan::New<v8::Array>();
}

void PopulateIndicators(
  std::vector<std::string> inputTypes,
  std::unordered_map<std::string, std::vector<double>> input,
  v8::Handle<v8::Object> indicators) {

  v8::Handle<v8::Array> keys = Nan::GetOwnPropertyNames(indicators)
    .ToLocalChecked();

  unsigned indicatorCount = keys->Length();

  for (unsigned i = 0; i < indicatorCount; ++i) {
    v8::Local<v8::String> key = v8::Local<v8::String>::Cast(keys->Get(i));
    std::string name = std::string(*v8::String::Utf8Value(key));

    inputTypes.push_back(name);
    input[name] = std::vector<double>();
    PopulateIndicatorValues(name, key, indicators, &input[name]);
  }
}

void PopulateIndicatorValues(
  std::string name,
  v8::Local<v8::String> key,
  v8::Handle<v8::Object> indicators,
  std::vector<double>* indicatorValues) {

  v8::Handle<v8::Array> values = v8::Handle<v8::Array>::Cast(
    indicators->Get(key));

  unsigned length = values->Length();
  for (unsigned i = 0; i < length; ++i) {
    indicatorValues->push_back(values->Get(i)->NumberValue());
  }
}
