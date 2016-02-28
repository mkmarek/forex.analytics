#pragma once
#include "../stdafx.h"
#include "TreeNode.h"

static const char * SignStrings[] = { ">", "<" };

class IndicatorTreeNode :
    public TreeNode
{
private:
const std::vector<BaseIndicator *>& _indicators;

public:
std::string indicator;
double value;
Sign sign;


IndicatorTreeNode(const std::vector<BaseIndicator *>& indicators);
~IndicatorTreeNode();

// Inherited via TreeNode
virtual bool Evaluate(
    const std::map<std::string, IndicatorData>& data) const override;

virtual void GenerateRandomValue() override;
virtual void GenerateRandomSign();
virtual void GenerateRandomIndicator();
virtual void GenerateRandomNumbericValue();

// Inherited via TreeNode
virtual void Copy(TreeNode * destination) const override;
virtual TreeNode * Copy() const override;

virtual void ToJs(v8::Local<v8::Object>& object, v8::Isolate * isolate)
const override;

static TreeNode * FromJs(
    const std::vector<BaseIndicator *>& indicators,
    const v8::Local<v8::Object>& input,
    v8::Isolate * isolate);
};
