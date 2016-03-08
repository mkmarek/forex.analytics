#include "../include/BinaryTreeFitness.h"
#include <thread>
#include <math.h>

BinaryTreeFitness::BinaryTreeFitness(
    FitnessFunction eval,
    const std::vector<IndicatorTuple>* dataSet) {
    this->eval = eval;
	this->dataSet = dataSet;
}

inline void multithreadedCalculation(
    unsigned begin,
    unsigned size,
    const std::vector<BinaryTreeChromosome *>* chromosomes,
    const std::vector<IndicatorTuple>* dataSet,
    FitnessFunction eval) {

    for (unsigned i = begin; i < begin + size; i++) {
        chromosomes->at(i)->setFitness(eval(FitnessFunctionArgs(chromosomes->at(i), dataSet)));
    }
}

void BinaryTreeFitness::CalculateFitness(
    const std::vector<BinaryTreeChromosome *>* chromosomes) const {
    unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
	size_t count = chromosomes->size();

    if (concurentThreadsSupported == 0) {
        for (unsigned i = 0; i < count; i++) {
            chromosomes->at(i)->setFitness(eval(FitnessFunctionArgs(chromosomes->at(i), this->dataSet)));
        }
    } else {

        std::vector<std::thread *> threads;

        double size = static_cast<double>(count) / concurentThreadsSupported;
        for (unsigned i = 0; i < concurentThreadsSupported; i++) {
            int begin = static_cast<int>(ceil(i * size));

            threads.push_back(new std::thread(
                                  multithreadedCalculation,
                                  begin,
                                  static_cast<int>(size),
                                  chromosomes,
                                  this->dataSet,
                                  eval
                                  ));
        }

        for (unsigned i = 0; i < concurentThreadsSupported; i++) {
            threads[i]->join();
            delete threads[i];
        }
    }
} // BinaryTreeFitness::CalculateFitness
