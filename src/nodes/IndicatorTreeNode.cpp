// Copyright 2016 Marek Magdziak

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "../../include/nodes/IndicatorTreeNode.h"

IndicatorTreeNode::IndicatorTreeNode(
  const std::vector<BaseIndicator*>& indicators) : _indicators(indicators), value(0)
{
	this->sign = Sign::Lt;
}

IndicatorTreeNode::~IndicatorTreeNode() {
}

bool IndicatorTreeNode::Evaluate(
  const std::map<std::string, IndicatorData>& data) const {
  for (unsigned long i = 0; i < this->_indicators.size(); i++) {
    if (this->_indicators[i]->Name == this->indicator) {
      return this->_indicators[i]->Evaluate(this->sign,
        this->value, data.at(this->indicator));
    }
  }

  return false;
}

void IndicatorTreeNode::GenerateRandomValue() {
  this->GenerateRandomSign();
  this->GenerateRandomIndicator();
}

void IndicatorTreeNode::GenerateRandomSign() {
    Sign old = this->sign;
    while (this->sign == old)
      this->sign = static_cast<Sign>(rand() % 2);
}

void IndicatorTreeNode::GenerateRandomIndicator() {
  this->indicator = this->_indicators[rand() % this->_indicators.size()]
    ->Name.c_str();

  this->GenerateRandomNumbericValue();
}

void IndicatorTreeNode::GenerateRandomNumbericValue() {
  for (unsigned long i = 0; i < this->_indicators.size(); i++) {
    if (this->_indicators[i]->Name == this->indicator) {
      this->value = this->_indicators[i]->getRandomValue();
    }
  }
}

void IndicatorTreeNode::Copy(TreeNode* destination) const {
  IndicatorTreeNode* dest = reinterpret_cast<IndicatorTreeNode*>(destination);

  dest->value = this->value;
  dest->indicator = this->indicator.c_str();
  dest->sign = this->sign;
}

TreeNode* IndicatorTreeNode::Copy() const {
  IndicatorTreeNode* dest = new IndicatorTreeNode(this->_indicators);

  dest->value = this->value;
  dest->indicator = this->indicator.c_str();
  dest->sign = this->sign;

  return dest;
}

void IndicatorTreeNode::ToJs(
  v8::Local<v8::Object>& object,
  v8::Isolate* isolate) const {
  object->Set(v8::String::NewFromUtf8(isolate, "indicator"),
    v8::String::NewFromUtf8(isolate, this->indicator.c_str()));

  object->Set(v8::String::NewFromUtf8(isolate, "sign"),
    v8::String::NewFromUtf8(isolate, SignStrings[this->sign]));

  object->Set(v8::String::NewFromUtf8(isolate, "value"),
    v8::Number::New(isolate, value));

  TreeNode::ToJs(object, isolate);
}

TreeNode* IndicatorTreeNode::FromJs(
  const std::vector<BaseIndicator*>& indicators,
  const v8::Local<v8::Object>& input,
  v8::Isolate* isolate) {

    if (!input->Has(v8::String::NewFromUtf8(isolate, "indicator")) ||
        !input->Has(v8::String::NewFromUtf8(isolate, "sign")) ||
        !input->Has(v8::String::NewFromUtf8(isolate, "value"))
        )
      return nullptr;

    std::string sign =
      std::string(*v8::String::Utf8Value(
        input->Get(v8::String::NewFromUtf8(isolate, "sign"))
        ->ToString()));

    Sign s = Sign::Gt;

    for (int i = 0; i < 2; i++) {
      if (sign == SignStrings[i]) {
        s = static_cast<Sign>(i);
      }
    }

    IndicatorTreeNode* node = new IndicatorTreeNode(indicators);
    node->sign = s;
    node->indicator = std::string(*v8::String::Utf8Value(
      input->Get(v8::String::NewFromUtf8(isolate, "indicator"))
      ->ToString()));

    node->value = input->Get(v8::String::NewFromUtf8(isolate, "value"))
      ->NumberValue();

    return node;
}
