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

bool IndicatorFactory::Register(const std::string name, const factoryMethod createMethod) {

   if (IndicatorFactory::registeredNames == nullptr) {
      IndicatorFactory::registeredNames = new std::map<std::string, factoryMethod>();
   }

  std::pair<std::map<std::string, factoryMethod>::iterator, bool> registeredPair =
   IndicatorFactory::registeredNames->insert(std::pair<std::string, factoryMethod>(name.c_str(), createMethod));

  return registeredPair.second;
}
