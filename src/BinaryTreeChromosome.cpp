#include "../include/BinaryTreeChromosome.h"
#include "../include/nodes/OperatorTreeNode.h"
#include "../include/nodes/IndicatorTreeNode.h"

#include <iostream>

BinaryTreeChromosome::BinaryTreeChromosome() {
	this->buy = nullptr;
	this->sell = nullptr;
	this->fitness = 0.0;
}

BinaryTreeChromosome::BinaryTreeChromosome(
	const BinaryTreeChromosome * chromosome) {

	this->buy = chromosome->buy->Copy();
	this->sell = chromosome->sell->Copy();
	this->fitness = chromosome->fitness;
}

BinaryTreeChromosome::~BinaryTreeChromosome() {
	if (this->buy != nullptr)
		delete this->buy;

	if (this->sell != nullptr)
		delete this->sell;

}

void BinaryTreeChromosome::setFitness(double value) {
	this->fitness = value;
}

double BinaryTreeChromosome::getFitness() const {
	return this->fitness;
}

void BinaryTreeChromosome::GenerateTree(
	int height,
	const std::vector<BaseIndicator *>& indicators) {
	this->_indicators = indicators;
	this->buy = GenerateTree(0, height);
	this->sell = GenerateTree(0, height);
}

const std::vector<BaseIndicator *>& BinaryTreeChromosome::getIndicators() const {
	return this->_indicators;
}

TreeNode * BinaryTreeChromosome::GenerateTree(int index, int height) const {
	
	TreeNode * ret;

	if (index < height - 1) {
		ret = new OperatorTreeNode();

		ret->GenerateRandomValue();
		ret->left = GenerateTree(index + 1, height);
		ret->right = GenerateTree(index + 1, height);
	}
	else {
		ret = new IndicatorTreeNode(this->_indicators);

		ret->GenerateRandomValue();
	}

	return ret;
}

bool BinaryTreeChromosome::operator<(
	const BinaryTreeChromosome * chromosome) const {
	return this->fitness < chromosome->fitness;
}

bool BinaryTreeChromosome::operator>(
	const BinaryTreeChromosome * chromosome) const {
	return this->fitness > chromosome->fitness;
}

BinaryTreeChromosome * BinaryTreeChromosome::FromJs(
	const std::vector<BaseIndicator *>& indicators,
	const v8::Local<v8::Object>& input,
	v8::Isolate * isolate) {
	BinaryTreeChromosome * chromosome = new BinaryTreeChromosome();

	chromosome->_indicators = indicators;

	v8::Handle<v8::Object> buy = v8::Handle<v8::Object>::Cast(
		input->Get(v8::String::NewFromUtf8(isolate, "buy")));
	v8::Handle<v8::Object> sell = v8::Handle<v8::Object>::Cast(
		input->Get(v8::String::NewFromUtf8(isolate, "sell")));

	chromosome->buy = TreeNode::FromJs(indicators, buy, isolate);
	chromosome->sell = TreeNode::FromJs(indicators, sell, isolate);

	return chromosome;
}
