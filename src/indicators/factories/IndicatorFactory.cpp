#include "nan.h"

#include "../../../include/indicators/factories/IndicatorFactory.h"

std::map<std::string, factoryMethod>* IndicatorFactory::registeredNames = nullptr;
std::map<std::string, BaseIndicator*>* IndicatorFactory::createdIndicators = nullptr;

BaseIndicator* IndicatorFactory::Create(const std::string name) {

	if (name == "")
		return nullptr;

	if (IndicatorFactory::createdIndicators == nullptr) {
		IndicatorFactory::createdIndicators = new std::map<std::string, BaseIndicator*>();
	}

	std::map<std::string, BaseIndicator*>::iterator createdIndicator =
		IndicatorFactory::createdIndicators->find(name);

	if (createdIndicator != IndicatorFactory::createdIndicators->end())
		return createdIndicator->second;

	std::map<std::string, factoryMethod>::iterator registeredPair =
		IndicatorFactory::registeredNames->find(name);

	if (registeredPair == IndicatorFactory::registeredNames->end())
		return nullptr;

	BaseIndicator* indicator = registeredPair->second();
	indicator->Name = name.c_str();

	IndicatorFactory::createdIndicators->insert(std::pair<std::string, BaseIndicator*>(name.c_str(), indicator));

	return indicator;
}

std::vector<BaseIndicator *> IndicatorFactory::CreateAll() {

	std::vector<BaseIndicator *> indicators;

	if (IndicatorFactory::createdIndicators == nullptr) {
		IndicatorFactory::createdIndicators = new std::map<std::string, BaseIndicator*>();
	}

	for (auto iter = registeredNames->begin(); iter != registeredNames->end(); ++iter) {

		BaseIndicator* indicator = iter->second();
		indicator->Name = iter->first.c_str();

		IndicatorFactory::createdIndicators->insert(std::pair<std::string, BaseIndicator*>(indicator->Name.c_str(), indicator));
		indicators.push_back(indicator);
	}

	return indicators;
}

bool IndicatorFactory::Register(const std::string name, const factoryMethod createMethod) {

	if (IndicatorFactory::registeredNames == nullptr) {
		IndicatorFactory::registeredNames = new std::map<std::string, factoryMethod>();
	}

	std::pair<std::map<std::string, factoryMethod>::iterator, bool> registeredPair =
		IndicatorFactory::registeredNames->insert(std::pair<std::string, factoryMethod>(name.c_str(), createMethod));

	return registeredPair.second;
}

std::vector<BaseIndicator *> IndicatorFactory::CreateFromArray(v8::Local<v8::Array> array)
{
	unsigned indicatorCount = array->Length();
	std::vector<std::string> indicatorNames;

	for (unsigned i = 0; i < indicatorCount; i++)
	{
		indicatorNames.push_back(std::string(*v8::String::Utf8Value(v8::Isolate::GetCurrent(), Nan::To<v8::String>(array->Get(i)).ToLocalChecked())));
	}

	std::vector<BaseIndicator *> indicators;

	for (unsigned long i = 0; i < indicatorNames.size(); i++)
	{
		BaseIndicator* indicator = IndicatorFactory::Create(indicatorNames[i]);
		indicators.push_back(indicator);
	}

	return indicators;
}