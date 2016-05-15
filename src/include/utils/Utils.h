#include <nan.h>
#include <node.h>
#include <unordered_map>

int getIntOrDefault(
  v8::Handle<v8::Object> object, const char* name, int def);

double getNumberOrDefault(
  v8::Handle<v8::Object> object, const char* name, double def);

v8::Handle<v8::Object> getObjectFromArguments(
  const Nan::FunctionCallbackInfo<v8::Value>& args, int index);

v8::Handle<v8::Array> getArrayFromArguments(
  const Nan::FunctionCallbackInfo<v8::Value>& args, int index);

void PopulateIndicators(
  std::vector<std::string> inputTypes,
  std::unordered_map<std::string, std::vector<double>> input,
  v8::Handle<v8::Object> indicators);

void PopulateIndicatorValues(
  std::string name,
  v8::Local<v8::String> key,
  v8::Handle<v8::Object> indicators,
  std::vector<double>* indicatorValues);


std::vector<std::unordered_map<std::string, double>> RemapMapArray(
  std::unordered_map<std::string, std::vector<double>>* input);
