#include "../../../include/export/findStrategy/findStrategyBaton.h"
#include "../../../include/utils/Utils.h"

FindStrategyBaton::FindStrategyBaton() {
  this->startingChromosome = nullptr;
  this->progress = nullptr;
  this->chromosome = nullptr;
  this->startingChromosome = nullptr;
  this->pipInDecimals = 0;
  this->crossoverProbability = 0;
  this->leafIndicatorMutationProbability = 0;
  this->logicalNodeMutationProbability = 0;
  this->leafSignMutationProbability = 0;
  this->leafValueMutationProbability = 0;
  this->selectionAmount = 0;
  this->generationCount = 0;
  this->populationCount = 0;
  this->errorMessage = "";
}

void FindStrategyBaton::PopulateFrom(v8::Handle<v8::Object> configuration) {

  this->populationCount = getIntOrDefault(
    configuration, "populationCount", DEFAULT_POPULATION_COUNT);
  this->generationCount = getIntOrDefault(
    configuration, "generationCount", DEFAULT_GENERATION_COUNT);
  this->selectionAmount = getIntOrDefault(
    configuration, "selectionAmount", DEFAULT_SELECTION_AMOUNT);
  this->leafValueMutationProbability = getNumberOrDefault(
    configuration, "leafValueMutationProbability",
    DEFAULT_LEAF_VALUE_MUTATION_PROBABILITY);
  this->leafSignMutationProbability = getNumberOrDefault(
    configuration, "leafSignMutationProbability",
    DEFAULT_LEAF_SIGN_MUTATION_PROBABILITY);
  this->logicalNodeMutationProbability = getNumberOrDefault(
    configuration, "logicalNodeMutationProbability",
    DEFAULT_LOGICAL_NODE_MUTATION_PROBABILITY);
  this->leafIndicatorMutationProbability = getNumberOrDefault(
    configuration, "leafIndicatorMutationProbability",
    DEFAULT_LEAF_INDICATOR_MUTATION_PROBABILITY);
  this->crossoverProbability = getNumberOrDefault(
    configuration, "crossoverProbability",
    DEFAULT_CROSSOVER_PROBABILITY);
  this->spread = getNumberOrDefault(
    configuration, "spread",
    DEFAULT_SPREAD);
  this->pipInDecimals = getNumberOrDefault(
    configuration, "spread",
    0.0f);

  std::unordered_map<std::string, std::vector<double>> indicatorValues;

	PopulateIndicators(this->inputTypes, indicatorValues,
    v8::Handle<v8::Object>::Cast(
        configuration->Get(Nan::New<v8::String>("indicators").ToLocalChecked())));

  this->input = RemapMapArray(&indicatorValues);

  if (configuration->Has(Nan::New<v8::String>("strategy").ToLocalChecked()))
  {
    this->startingChromosome = BinaryTreeChromosome::FromJs(
      this->inputTypes,
      v8::Handle<v8::Object>::Cast(
        configuration->Get(Nan::New<v8::String>("strategy").ToLocalChecked()))
    );
  }
}
