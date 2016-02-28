#include "../include/BinaryTreeFitness.h"
#include <thread>
#include <math.h>

BinaryTreeFitness::BinaryTreeFitness(
    FitnessFunction eval,
    const std::vector<std::map<std::string, IndicatorData> >& dataSet) : dataSet(dataSet){
    this->eval = eval;
}

void multithreadedCalculation(
    unsigned begin,
    unsigned size,
    const std::vector<BinaryTreeChromosome *>& chromosomes,
    const std::vector<std::map<std::string, IndicatorData> >& dataSet,
    FitnessFunction eval) {

    for (unsigned i = begin; i < begin + size; i++) {
        chromosomes[i]->setFitness(eval(chromosomes[i], dataSet));
    }
}

void BinaryTreeFitness::CalculateFitness(
    const std::vector<BinaryTreeChromosome *>& chromosomes) const {
    unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
    unsigned count = chromosomes.size();

    if (concurentThreadsSupported == 0) {
        for (unsigned i = 0; i < count; i++) {
            chromosomes[i]->setFitness(eval(chromosomes[i], this->dataSet));
        }
    } else {

        std::vector<std::thread *> threads;

        double size = ((double)count) / concurentThreadsSupported;
        for (unsigned i = 0; i < concurentThreadsSupported; i++) {
            int begin = ceil(i * size);

            threads.push_back(new std::thread(
                                  multithreadedCalculation,
                                  begin,
                                  (int)size,
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
