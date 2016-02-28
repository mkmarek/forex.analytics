#include "../../../include/indicators/factories/IndicatorFactory.h"
#include "../../../include/indicators/Indicator.h"

#include <iostream>

std::map<std::string, factoryMethod>* IndicatorFactory::registeredNames = NULL;
std::map<std::string, BaseIndicator*>* IndicatorFactory::createdIndicators = NULL;

 BaseIndicator* IndicatorFactory::Create(const std::string name) {

   if (name == "")
    return NULL;

   if (IndicatorFactory::createdIndicators == NULL) {
      IndicatorFactory::createdIndicators = new std::map<std::string, BaseIndicator*>();
   }

   std::map<std::string, BaseIndicator*>::iterator createdIndicator =
    IndicatorFactory::createdIndicators->find(name);

    if (createdIndicator != IndicatorFactory::createdIndicators->end())
      return createdIndicator->second;

      std::cout << "Creating " << name << std::endl;

   std::map<std::string, factoryMethod>::iterator registeredPair =
    IndicatorFactory::registeredNames->find(name);

    if (registeredPair == IndicatorFactory::registeredNames->end())
      return NULL;

    BaseIndicator* indicator = registeredPair->second();
    indicator->Name = name.c_str();

    IndicatorFactory::createdIndicators->insert(std::pair<std::string, BaseIndicator*>(name.c_str(), indicator));

    return indicator;
 }

bool IndicatorFactory::Register(const std::string name, const factoryMethod createMethod) {

  std::cout << "Registering " << name << std::endl;

   if (IndicatorFactory::registeredNames == NULL) {
      IndicatorFactory::registeredNames = new std::map<std::string, factoryMethod>();
   }

  std::pair<std::map<std::string, factoryMethod>::iterator, bool> registeredPair =
   IndicatorFactory::registeredNames->insert(std::pair<std::string, factoryMethod>(name.c_str(), createMethod));

  return registeredPair.second;
}
