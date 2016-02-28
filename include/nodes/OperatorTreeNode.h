#pragma once

#include "../stdafx.h"
#include "TreeNode.h"

enum Operator {  And = 0,  Or = 1 };
static const char * OperatorStrings[] = { "And", "Or" };

class OperatorTreeNode :
    public TreeNode
{
public:
Operator value;

OperatorTreeNode();
~OperatorTreeNode();

// Inherited via TreeNode
virtual bool Evaluate(
    const std::map<std::string, IndicatorData>& data) const override;

virtual void GenerateRandomValue() override;

// Inherited via TreeNode
virtual void Copy(TreeNode * destination) const override;
virtual TreeNode * Copy() const override;

virtual void ToJs(v8::Local<v8::Object>& object, v8::Isolate * isolate)
const override;

static TreeNode * FromJs(const v8::Local<v8::Object>& input, v8::Isolate * isolate);
};
