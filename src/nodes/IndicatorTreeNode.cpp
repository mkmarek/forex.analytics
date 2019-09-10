// Copyright 2016 Marek Magdziak

#include <iostream>
#include <vector>
#include <map>
#include "nan.h"

#include "../../include/nodes/IndicatorTreeNode.h"

IndicatorTreeNode::IndicatorTreeNode(
  const std::vector<BaseIndicator*>& indicators) : _indicators(indicators), value(0)
{
	this->sign = Sign::Lt;

	sign_uniform_dist = std::uniform_int_distribution<int>(0, 1);
	indicator_uniform_dist = std::uniform_int_distribution<int>(0, indicators.size() - 1);
}

IndicatorTreeNode::~IndicatorTreeNode() {
}

bool IndicatorTreeNode::Evaluate(
  const std::map<std::string, IndicatorData>& data) const {
  for (unsigned long i = 0; i < this->_indicators.size(); i++) {
    if (this->_indicators[i]->Name == this->indicator) {

		switch (sign) {
		case Sign::Gt:
			return (value / 1000) > data.at(this->indicator).data;
		case Sign::Lt:
			return value / 1000 < data.at(this->indicator).data;
		}

		return false;
    }
  }

  return false;
}

void IndicatorTreeNode::GenerateRandomValue() {
  this->GenerateRandomSign();
  this->GenerateRandomIndicator();
}

void IndicatorTreeNode::GenerateRandomSign() {
    this->sign = static_cast<Sign>(sign_uniform_dist(engine));
}

void IndicatorTreeNode::GenerateRandomIndicator() {
  this->indicator = this->_indicators[indicator_uniform_dist(engine)]
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
  v8::Local<v8::Object>& object) const {
  object->Set(Nan::New<v8::String>("indicator").ToLocalChecked(),
    Nan::New<v8::String>(this->indicator.c_str()).ToLocalChecked());

  object->Set(Nan::New<v8::String>("sign").ToLocalChecked(),
    Nan::New<v8::String>(SignStrings[this->sign]).ToLocalChecked());

  object->Set(Nan::New<v8::String>("value").ToLocalChecked(),
    Nan::New<v8::Number>(value));

  TreeNode::ToJs(object);
}

TreeNode* IndicatorTreeNode::FromJs(
  const std::vector<BaseIndicator*>& indicators,
  const v8::Local<v8::Object>& input) {


  if ((!Nan::Has(input, Nan::New<v8::String>("indicator").ToLocalChecked()).FromJust()) ||
      (!Nan::Has(input, Nan::New<v8::String>("sign").ToLocalChecked()).FromJust()) ||
      (!Nan::Has(input, Nan::New<v8::String>("value").ToLocalChecked()).FromJust()))
    return nullptr;

  std::string sign = std::string(*v8::String::Utf8Value(
      v8::Isolate::GetCurrent(),
      input->Get(Nan::New<v8::String>("sign").ToLocalChecked())));
    Sign s = Sign::Gt;

    for (int i = 0; i < 2; i++) {
      if (sign == SignStrings[i]) {
        s = static_cast<Sign>(i);
      }
    }

    IndicatorTreeNode* node = new IndicatorTreeNode(indicators);
    node->sign = s;
    node->indicator = std::string(*v8::String::Utf8Value(
      v8::Isolate::GetCurrent(),
      input->Get(Nan::New<v8::String>("indicator").ToLocalChecked())));

    node->value = input->Get(Nan::New<v8::String>("value").ToLocalChecked())->NumberValue(Nan::GetCurrentContext()).ToChecked();

    return node;
}
