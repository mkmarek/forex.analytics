#pragma once

#include "../../stdafx.h"
#include "../../indicators/BaseIndicator.h"

/**
 * The factory method type which should return a pointer to a BaseIndicator
 */
typedef BaseIndicator * (* factoryMethod)();

/**
 * Factory class for creating new indicator instances
 * Every indicator is held as a singleton for preventing memory leaks
 */
class IndicatorFactory
{
public:

/**
 * Returns an instance of indicator based on the name
 * @param  name The name of desired indicator
 * @return      Pointer to a BaseIndicator instance
 */
static BaseIndicator * Create(const std::string name);

/**
* Returns a vector containing all indicators in the system
*/
static std::vector<BaseIndicator *> CreateAll();

/**
 * Registers an indicator factory method used for creating new indicators
 * @param  name         Name of the registered indicator
 * @param  createMethod The indicator factory method
 * @return              Status whether indicator was successfully registered
 */
static bool Register(const std::string name, const factoryMethod createMethod);\

protected:

/**
 * Map tracking registered indicators and their factory methods
 */
static std::map<std::string, factoryMethod> * registeredNames;

/**
 * Map tracking created indicators and their instances for further reuse
 */
static std::map<std::string, BaseIndicator *> * createdIndicators;
};
